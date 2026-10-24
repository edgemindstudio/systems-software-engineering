#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int server = socket(AF_INET6, SOCK_STREAM, 0);
    struct sockaddr_in6 addr;
    socklen_t len = (socklen_t)sizeof addr;
    pid_t child;
    int status = 0;

    if (server < 0) { perror("socket"); return 1; }

    memset(&addr, 0, sizeof addr);
    addr.sin6_family = AF_INET6;
    addr.sin6_addr = in6addr_loopback;
    addr.sin6_port = htons(0);

    if (bind(server, (struct sockaddr *)&addr, sizeof addr) != 0 ||
        listen(server, 1) != 0 ||
        getsockname(server, (struct sockaddr *)&addr, &len) != 0) {
        perror("ipv6 server setup");
        return 1;
    }

    child = fork();
    if (child < 0) return 1;

    if (child == 0) {
        int client = socket(AF_INET6, SOCK_STREAM, 0);
        if (client < 0) _exit(2);
        if (connect(client, (struct sockaddr *)&addr, sizeof addr) != 0) _exit(3);
        if (send(client, "ipv6-ok", 7U, 0) != 7) _exit(4);
        close(client);
        _exit(0);
    }

    {
        int peer = accept(server, NULL, NULL);
        char buf[32];
        ssize_t n;
        if (peer < 0) return 1;
        n = recv(peer, buf, sizeof buf - 1U, 0);
        if (n <= 0) return 1;
        buf[(size_t)n] = '\0';
        printf("ipv6_received=%s\n", buf);
        printf("ipv6_loopback_verified=%s\n",
               strcmp(buf, "ipv6-ok") == 0 ? "yes" : "no");
        close(peer);
    }

    close(server);
    if (waitpid(child, &status, 0) < 0) return 1;
    return WIFEXITED(status) && WEXITSTATUS(status) == 0 ? 0 : 1;
}
