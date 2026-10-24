#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

enum { CLIENTS = 4 };

struct worker_arg {
    int fd;
    int index;
};

static void *worker(void *arg)
{
    struct worker_arg *wa = arg;
    char buf[64];
    ssize_t n = recv(wa->fd, buf, sizeof buf - 1U, 0);
    if (n > 0) {
        buf[(size_t)n] = '\0';
        printf("worker=%d received=%s\n", wa->index, buf);
        fflush(stdout);
        (void)send(wa->fd, "ack", 3U, 0);
    }
    close(wa->fd);
    return NULL;
}

int main(void)
{
    int server = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    socklen_t len = (socklen_t)sizeof addr;
    pid_t child;
    int status = 0;

    if (server < 0) return 1;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(0);

    if (bind(server, (struct sockaddr *)&addr, sizeof addr) != 0 ||
        listen(server, CLIENTS) != 0 ||
        getsockname(server, (struct sockaddr *)&addr, &len) != 0) return 1;

    child = fork();
    if (child < 0) return 1;

    if (child == 0) {
        for (int i = 0; i < CLIENTS; ++i) {
            int client = socket(AF_INET, SOCK_STREAM, 0);
            char msg[32];
            char ack[8];
            ssize_t n;
            if (client < 0) _exit(2);
            if (connect(client, (struct sockaddr *)&addr, sizeof addr) != 0) _exit(3);
            (void)snprintf(msg, sizeof msg, "client-%d", i);
            if (send(client, msg, strlen(msg), 0) < 0) _exit(4);
            n = recv(client, ack, sizeof ack - 1U, 0);
            if (n <= 0) _exit(5);
            ack[(size_t)n] = '\0';
            if (strcmp(ack, "ack") != 0) _exit(6);
            close(client);
        }
        _exit(0);
    }

    {
        pthread_t threads[CLIENTS];
        struct worker_arg args[CLIENTS];

        for (int i = 0; i < CLIENTS; ++i) {
            int peer = accept(server, NULL, NULL);
            if (peer < 0) return 1;
            args[i].fd = peer;
            args[i].index = i;
            if (pthread_create(&threads[i], NULL, worker, &args[i]) != 0) return 1;
        }

        for (int i = 0; i < CLIENTS; ++i) {
            if (pthread_join(threads[i], NULL) != 0) return 1;
        }
    }

    close(server);
    if (waitpid(child, &status, 0) < 0) return 1;
    printf("clients_handled=%d\n", CLIENTS);
    printf("threaded_service_verified=%s\n",
           WIFEXITED(status) && WEXITSTATUS(status) == 0 ? "yes" : "no");
    return 0;
}
