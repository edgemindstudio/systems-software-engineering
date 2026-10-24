#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void)
{
    int pair[2];
    int flags;
    char buf[64];
    ssize_t n;
    struct pollfd pfd;

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, pair) != 0) {
        perror("socketpair");
        return 1;
    }

    flags = fcntl(pair[0], F_GETFL, 0);
    if (flags < 0 || fcntl(pair[0], F_SETFL, flags | O_NONBLOCK) != 0) {
        perror("fcntl");
        return 1;
    }

    n = recv(pair[0], buf, sizeof buf, 0);
    printf("initial_recv_eagain=%s\n",
           n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK) ? "yes" : "no");

    if (send(pair[1], "poll-ready", 10U, 0) != 10) return 1;

    pfd.fd = pair[0];
    pfd.events = POLLIN;
    pfd.revents = 0;

    if (poll(&pfd, 1U, 1000) <= 0) {
        perror("poll");
        return 1;
    }

    n = recv(pair[0], buf, sizeof buf - 1U, 0);
    if (n <= 0) return 1;
    buf[(size_t)n] = '\0';

    printf("poll_readable=%s\n", (pfd.revents & POLLIN) != 0 ? "yes" : "no");
    printf("received=%s\n", buf);
    printf("nonblocking_poll_verified=%s\n",
           strcmp(buf, "poll-ready") == 0 ? "yes" : "no");

    close(pair[0]);
    close(pair[1]);
    return 0;
}
