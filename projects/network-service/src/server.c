#include "netservice/protocol.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
    int *fds;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t size;
    int stopping;

    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;

    size_t accepted;
    size_t completed;
    size_t protocol_errors;
} connection_queue_t;

typedef struct {
    connection_queue_t *queue;
} worker_ctx_t;

static volatile sig_atomic_t stop_requested = 0;
static volatile sig_atomic_t listening_fd = -1;

static void handle_signal(int signo)
{
    (void)signo;
    stop_requested = 1;

    if (listening_fd >= 0) {
        (void)close((int)listening_fd);
        listening_fd = -1;
    }
}

static int install_signal_handlers(void)
{
    struct sigaction action;
    action.sa_handler = handle_signal;
    action.sa_flags = 0;

    if (sigemptyset(&action.sa_mask) != 0) {
        return -1;
    }

    if (sigaction(SIGINT, &action, NULL) != 0) {
        return -1;
    }
    if (sigaction(SIGTERM, &action, NULL) != 0) {
        return -1;
    }

    struct sigaction ignore;
    ignore.sa_handler = SIG_IGN;
    ignore.sa_flags = 0;
    if (sigemptyset(&ignore.sa_mask) != 0) {
        return -1;
    }
    if (sigaction(SIGPIPE, &ignore, NULL) != 0) {
        return -1;
    }

    return 0;
}

static int queue_init(connection_queue_t *queue, size_t capacity)
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

static void queue_destroy(connection_queue_t *queue)
{
    (void)pthread_cond_destroy(&queue->not_full);
    (void)pthread_cond_destroy(&queue->not_empty);
    (void)pthread_mutex_destroy(&queue->mutex);
    free(queue->fds);
}

static int queue_push(connection_queue_t *queue, int fd)
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

static int queue_pop(connection_queue_t *queue, int *fd_out)
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

static void queue_stop(connection_queue_t *queue)
{
    (void)pthread_mutex_lock(&queue->mutex);
    queue->stopping = 1;
    (void)pthread_cond_broadcast(&queue->not_empty);
    (void)pthread_cond_broadcast(&queue->not_full);
    (void)pthread_mutex_unlock(&queue->mutex);
}

static void mark_completed(connection_queue_t *queue, int protocol_error)
{
    (void)pthread_mutex_lock(&queue->mutex);
    ++queue->completed;
    if (protocol_error != 0) {
        ++queue->protocol_errors;
    }
    (void)pthread_mutex_unlock(&queue->mutex);
}

static void serve_connection(int fd, connection_queue_t *queue)
{
    unsigned char buffer[NETSERVICE_MAX_FRAME];

    for (;;) {
        uint32_t length = 0U;
        int rc = netservice_read_frame(fd,
                                       buffer,
                                       sizeof buffer,
                                       &length);

        if (rc == 0) {
            mark_completed(queue, 0);
            return;
        }

        if (rc < 0) {
            mark_completed(queue, 1);
            return;
        }

        if (netservice_write_frame(fd, buffer, length) != 0) {
            mark_completed(queue, 0);
            return;
        }
    }
}

static void *worker_main(void *context)
{
    worker_ctx_t *worker = context;

    for (;;) {
        int fd = -1;
        int rc = queue_pop(worker->queue, &fd);

        if (rc == 0) {
            break;
        }

        serve_connection(fd, worker->queue);
        (void)close(fd);
    }

    return NULL;
}

static int parse_size(const char *text, size_t *out)
{
    char *end = NULL;
    errno = 0;
    unsigned long value = strtoul(text, &end, 10);

    if (errno != 0 || end == text || *end != '\0' || value == 0UL) {
        return -1;
    }

    *out = (size_t)value;
    return 0;
}

static int parse_port(const char *text, uint16_t *out)
{
    char *end = NULL;
    errno = 0;
    unsigned long value = strtoul(text, &end, 10);

    if (errno != 0 || end == text || *end != '\0' || value > 65535UL) {
        return -1;
    }

    *out = (uint16_t)value;
    return 0;
}

static int write_port_file(const char *path, uint16_t port)
{
    if (path == NULL) {
        return 0;
    }

    FILE *file = fopen(path, "w");
    if (file == NULL) {
        return -1;
    }

    if (fprintf(file, "%u\n", (unsigned int)port) < 0) {
        (void)fclose(file);
        return -1;
    }

    return fclose(file);
}

int main(int argc, char **argv)
{
    uint16_t requested_port = 0U;
    size_t worker_count = 4U;
    size_t queue_capacity = 32U;
    const char *port_file = NULL;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--port") == 0 && i + 1 < argc) {
            if (parse_port(argv[++i], &requested_port) != 0) {
                return 2;
            }
        } else if (strcmp(argv[i], "--workers") == 0 && i + 1 < argc) {
            if (parse_size(argv[++i], &worker_count) != 0) {
                return 2;
            }
        } else if (strcmp(argv[i], "--queue") == 0 && i + 1 < argc) {
            if (parse_size(argv[++i], &queue_capacity) != 0) {
                return 2;
            }
        } else if (strcmp(argv[i], "--port-file") == 0 && i + 1 < argc) {
            port_file = argv[++i];
        } else {
            (void)fprintf(stderr,
                          "usage: netservice-server "
                          "[--port N] [--workers N] [--queue N] "
                          "[--port-file PATH]\n");
            return 2;
        }
    }

    if (install_signal_handlers() != 0) {
        perror("sigaction");
        return 1;
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    int reuse = 1;
    if (setsockopt(server_fd,
                   SOL_SOCKET,
                   SO_REUSEADDR,
                   &reuse,
                   sizeof reuse) != 0) {
        perror("setsockopt");
        (void)close(server_fd);
        return 1;
    }

    struct sockaddr_in address;
    (void)memset(&address, 0, sizeof address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(requested_port);

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
        return 1;
    }

    socklen_t address_length = sizeof address;
    if (getsockname(server_fd,
                    (struct sockaddr *)&address,
                    &address_length) != 0) {
        perror("getsockname");
        (void)close(server_fd);
        return 1;
    }

    uint16_t actual_port = ntohs(address.sin_port);

    connection_queue_t queue;
    if (queue_init(&queue, queue_capacity) != 0) {
        perror("queue_init");
        (void)close(server_fd);
        return 1;
    }

    pthread_t *workers = calloc(worker_count, sizeof *workers);
    worker_ctx_t *contexts = calloc(worker_count, sizeof *contexts);
    if (workers == NULL || contexts == NULL) {
        free(contexts);
        free(workers);
        queue_destroy(&queue);
        (void)close(server_fd);
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
        return 1;
    }

    listening_fd = server_fd;

    if (write_port_file(port_file, actual_port) != 0) {
        perror("port-file");
        stop_requested = 1;
    }

    (void)printf("listening_address=127.0.0.1\n");
    (void)printf("listening_port=%u\n", (unsigned int)actual_port);
    (void)printf("workers=%zu\n", worker_count);
    (void)printf("queue_capacity=%zu\n", queue_capacity);
    (void)fflush(stdout);

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

    if (listening_fd >= 0) {
        (void)close((int)listening_fd);
        listening_fd = -1;
    }

    queue_stop(&queue);

    for (size_t i = 0U; i < worker_count; ++i) {
        (void)pthread_join(workers[i], NULL);
    }

    (void)pthread_mutex_lock(&queue.mutex);
    size_t accepted = queue.accepted;
    size_t completed = queue.completed;
    size_t protocol_errors = queue.protocol_errors;
    (void)pthread_mutex_unlock(&queue.mutex);

    (void)printf("accepted_connections=%zu\n", accepted);
    (void)printf("completed_connections=%zu\n", completed);
    (void)printf("protocol_errors=%zu\n", protocol_errors);
    (void)printf("shutdown=graceful\n");

    free(contexts);
    free(workers);
    queue_destroy(&queue);
    return 0;
}
