#include "sysdiag/diagnostics.h"
#include "sysdiag/protocol.h"

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

typedef struct {
    int *fds;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t size;
    int stopping;

    size_t accepted;
    size_t completed;
    size_t request_errors;

    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} fd_queue_t;

typedef struct {
    fd_queue_t *queue;
} worker_ctx_t;

static volatile sig_atomic_t stop_requested = 0;
static volatile sig_atomic_t listen_fd_global = -1;

static void signal_handler(int signo)
{
    (void)signo;
    stop_requested = 1;

    if (listen_fd_global >= 0) {
        (void)close((int)listen_fd_global);
        listen_fd_global = -1;
    }
}

static int install_signals(void)
{
    struct sigaction action;
    action.sa_handler = signal_handler;
    action.sa_flags = 0;
    if (sigemptyset(&action.sa_mask) != 0) {
        return -1;
    }

    if (sigaction(SIGINT, &action, NULL) != 0 ||
        sigaction(SIGTERM, &action, NULL) != 0) {
        return -1;
    }

    struct sigaction ignore;
    ignore.sa_handler = SIG_IGN;
    ignore.sa_flags = 0;
    if (sigemptyset(&ignore.sa_mask) != 0 ||
        sigaction(SIGPIPE, &ignore, NULL) != 0) {
        return -1;
    }

    return 0;
}

static int queue_init(fd_queue_t *queue, size_t capacity)
{
    (void)memset(queue, 0, sizeof *queue);
    queue->fds = calloc(capacity, sizeof *queue->fds);
    if (queue->fds == NULL) {
        return -1;
    }
    queue->capacity = capacity;

    if (pthread_mutex_init(&queue->mutex, NULL) != 0 ||
        pthread_cond_init(&queue->not_empty, NULL) != 0 ||
        pthread_cond_init(&queue->not_full, NULL) != 0) {
        free(queue->fds);
        return -1;
    }

    return 0;
}

static void queue_destroy(fd_queue_t *queue)
{
    (void)pthread_cond_destroy(&queue->not_full);
    (void)pthread_cond_destroy(&queue->not_empty);
    (void)pthread_mutex_destroy(&queue->mutex);
    free(queue->fds);
}

static int queue_push(fd_queue_t *queue, int fd)
{
    (void)pthread_mutex_lock(&queue->mutex);

    while (queue->size == queue->capacity && queue->stopping == 0) {
        (void)pthread_cond_wait(&queue->not_full, &queue->mutex);
    }

    if (queue->stopping != 0) {
        (void)pthread_mutex_unlock(&queue->mutex);
        return -1;
    }

    queue->fds[queue->tail] = fd;
    queue->tail = (queue->tail + 1U) % queue->capacity;
    ++queue->size;
    ++queue->accepted;

    (void)pthread_cond_signal(&queue->not_empty);
    (void)pthread_mutex_unlock(&queue->mutex);
    return 0;
}

static int queue_pop(fd_queue_t *queue, int *fd_out)
{
    (void)pthread_mutex_lock(&queue->mutex);

    while (queue->size == 0U && queue->stopping == 0) {
        (void)pthread_cond_wait(&queue->not_empty, &queue->mutex);
    }

    if (queue->size == 0U && queue->stopping != 0) {
        (void)pthread_mutex_unlock(&queue->mutex);
        return 0;
    }

    *fd_out = queue->fds[queue->head];
    queue->head = (queue->head + 1U) % queue->capacity;
    --queue->size;

    (void)pthread_cond_signal(&queue->not_full);
    (void)pthread_mutex_unlock(&queue->mutex);
    return 1;
}

static void queue_stop(fd_queue_t *queue)
{
    (void)pthread_mutex_lock(&queue->mutex);
    queue->stopping = 1;
    (void)pthread_cond_broadcast(&queue->not_empty);
    (void)pthread_cond_broadcast(&queue->not_full);
    (void)pthread_mutex_unlock(&queue->mutex);
}

static void queue_complete(fd_queue_t *queue, int error)
{
    (void)pthread_mutex_lock(&queue->mutex);
    ++queue->completed;
    if (error != 0) {
        ++queue->request_errors;
    }
    (void)pthread_mutex_unlock(&queue->mutex);
}

static void service_client(int fd, fd_queue_t *queue)
{
    unsigned char request[SYSDIAG_MAX_REQUEST + 1U];
    uint32_t length = 0U;

    int rc = sysdiag_read_frame(fd,
                                request,
                                SYSDIAG_MAX_REQUEST,
                                &length);
    if (rc != 1) {
        queue_complete(queue, 1);
        return;
    }

    request[length] = '\0';

    if (memchr(request, '\0', length) != NULL) {
        static const char bad[] =
            "ERR code=EINVAL message=binary-request\n";
        (void)sysdiag_write_frame(fd,
                                  (const unsigned char *)bad,
                                  (uint32_t)(sizeof bad - 1U));
        queue_complete(queue, 1);
        return;
    }

    char response[SYSDIAG_MAX_RESPONSE];
    int command_error =
        sysdiag_handle_request((const char *)request,
                               response,
                               sizeof response);

    size_t response_length = strlen(response);
    if (response_length > UINT32_MAX ||
        sysdiag_write_frame(fd,
                            (const unsigned char *)response,
                            (uint32_t)response_length) != 0) {
        queue_complete(queue, 1);
        return;
    }

    queue_complete(queue, command_error);
}

static void *worker_main(void *context)
{
    worker_ctx_t *worker = context;

    for (;;) {
        int fd = -1;

        if (queue_pop(worker->queue, &fd) == 0) {
            break;
        }

        service_client(fd, worker->queue);
        (void)close(fd);
    }

    return NULL;
}

static int parse_size(const char *text, size_t *out)
{
    char *end = NULL;
    errno = 0;
    unsigned long value = strtoul(text, &end, 10);

    if (errno != 0 || end == text ||
        *end != '\0' || value == 0UL) {
        return -1;
    }

    *out = (size_t)value;
    return 0;
}

int main(int argc, char **argv)
{
    const char *socket_path = NULL;
    size_t worker_count = 4U;
    size_t queue_capacity = 32U;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--socket") == 0 && i + 1 < argc) {
            socket_path = argv[++i];
        } else if (strcmp(argv[i], "--workers") == 0 && i + 1 < argc) {
            if (parse_size(argv[++i], &worker_count) != 0) {
                return 2;
            }
        } else if (strcmp(argv[i], "--queue") == 0 && i + 1 < argc) {
            if (parse_size(argv[++i], &queue_capacity) != 0) {
                return 2;
            }
        } else {
            fprintf(stderr,
                    "usage: sysdiag-daemon --socket PATH "
                    "[--workers N] [--queue N]\n");
            return 2;
        }
    }

    if (socket_path == NULL ||
        strlen(socket_path) >= sizeof(((struct sockaddr_un *)0)->sun_path)) {
        fprintf(stderr, "invalid socket path\n");
        return 2;
    }

    if (install_signals() != 0) {
        perror("sigaction");
        return 1;
    }

    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    (void)unlink(socket_path);

    struct sockaddr_un address;
    (void)memset(&address, 0, sizeof address);
    address.sun_family = AF_UNIX;
    (void)snprintf(address.sun_path,
                   sizeof address.sun_path,
                   "%s",
                   socket_path);

    if (bind(server_fd,
             (struct sockaddr *)&address,
             sizeof address) != 0) {
        perror("bind");
        (void)close(server_fd);
        return 1;
    }

    if (listen(server_fd, 64) != 0) {
        perror("listen");
        (void)close(server_fd);
        (void)unlink(socket_path);
        return 1;
    }

    fd_queue_t queue;
    if (queue_init(&queue, queue_capacity) != 0) {
        (void)close(server_fd);
        (void)unlink(socket_path);
        return 1;
    }

    pthread_t *workers = calloc(worker_count, sizeof *workers);
    worker_ctx_t *contexts = calloc(worker_count, sizeof *contexts);

    if (workers == NULL || contexts == NULL) {
        free(contexts);
        free(workers);
        queue_destroy(&queue);
        (void)close(server_fd);
        (void)unlink(socket_path);
        return 1;
    }

    size_t started = 0U;
    for (; started < worker_count; ++started) {
        contexts[started].queue = &queue;

        if (pthread_create(&workers[started],
                           NULL,
                           worker_main,
                           &contexts[started]) != 0) {
            break;
        }
    }

    if (started != worker_count) {
        queue_stop(&queue);
        for (size_t i = 0U; i < started; ++i) {
            (void)pthread_join(workers[i], NULL);
        }
        free(contexts);
        free(workers);
        queue_destroy(&queue);
        (void)close(server_fd);
        (void)unlink(socket_path);
        return 1;
    }

    listen_fd_global = server_fd;

    printf("socket=%s\n", socket_path);
    printf("workers=%zu\n", worker_count);
    printf("queue_capacity=%zu\n", queue_capacity);
    fflush(stdout);

    while (stop_requested == 0) {
        int client_fd = accept(server_fd, NULL, NULL);

        if (client_fd < 0) {
            if (errno == EINTR) {
                continue;
            }
            if (stop_requested != 0 || errno == EBADF) {
                break;
            }
            perror("accept");
            break;
        }

        if (queue_push(&queue, client_fd) != 0) {
            (void)close(client_fd);
            break;
        }
    }

    if (listen_fd_global >= 0) {
        (void)close((int)listen_fd_global);
        listen_fd_global = -1;
    }

    queue_stop(&queue);

    for (size_t i = 0U; i < worker_count; ++i) {
        (void)pthread_join(workers[i], NULL);
    }

    (void)pthread_mutex_lock(&queue.mutex);
    size_t accepted = queue.accepted;
    size_t completed = queue.completed;
    size_t errors = queue.request_errors;
    (void)pthread_mutex_unlock(&queue.mutex);

    printf("accepted_requests=%zu\n", accepted);
    printf("completed_requests=%zu\n", completed);
    printf("request_errors=%zu\n", errors);
    printf("shutdown=graceful\n");

    free(contexts);
    free(workers);
    queue_destroy(&queue);
    (void)unlink(socket_path);
    return 0;
}
