#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int write_all(int fd, const char *buf, size_t len)
{
    size_t done = 0U;
    while (done < len) {
        ssize_t n = send(fd, buf + done, len - done, 0);
        if (n < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        done += (size_t)n;
    }
    return 0;
}

int main(void)
{
    int server = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    socklen_t addrlen = (socklen_t)sizeof addr;
    pid_t child;
    int status = 0;

    if (server < 0) { perror("socket"); return 1; }

    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(0);

    if (bind(server, (struct sockaddr *)&addr, sizeof addr) != 0 ||
        listen(server, 4) != 0 ||
        getsockname(server, (struct sockaddr *)&addr, &addrlen) != 0) {
        perror("server setup");
        return 1;
    }

    printf("server_port=%u\n", (unsigned)ntohs(addr.sin_port));
    fflush(stdout);

    child = fork();
    if (child < 0) { perror("fork"); return 1; }

    if (child == 0) {
        int client = socket(AF_INET, SOCK_STREAM, 0);
        char buf[64];
        ssize_t n;
        if (client < 0) _exit(2);
        if (connect(client, (struct sockaddr *)&addr, sizeof addr) != 0) _exit(3);
        if (write_all(client, "tcp-echo", 8U) != 0) _exit(4);
        n = recv(client, buf, sizeof buf - 1U, 0);
        if (n <= 0) _exit(5);
        buf[(size_t)n] = '\0';
        printf("client_received=%s\n", buf);
        fflush(stdout);
        close(client);
        _exit(strcmp(buf, "tcp-echo") == 0 ? 0 : 6);
    }

    {
        int peer = accept(server, NULL, NULL);
        char buf[64];
        ssize_t n;
        if (peer < 0) { perror("accept"); return 1; }
        n = recv(peer, buf, sizeof buf - 1U, 0);
        if (n <= 0) { perror("recv"); return 1; }
        buf[(size_t)n] = '\0';
        printf("server_received=%s\n", buf);
        if (write_all(peer, buf, (size_t)n) != 0) return 1;
        close(peer);
    }

    close(server);
    if (waitpid(child, &status, 0) < 0) return 1;
    printf("tcp_round_trip_verified=%s\n",
           WIFEXITED(status) && WEXITSTATUS(status) == 0 ? "yes" : "no");
    return 0;
}
