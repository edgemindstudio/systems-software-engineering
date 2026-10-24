#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int write_all(int fd, const void *buf, size_t len)
{
    const unsigned char *p = buf;
    size_t done = 0U;
    while (done < len) {
        ssize_t n = send(fd, p + done, len - done, 0);
        if (n < 0) { if (errno == EINTR) continue; return -1; }
        done += (size_t)n;
    }
    return 0;
}

static int read_exact(int fd, void *buf, size_t len)
{
    unsigned char *p = buf;
    size_t done = 0U;
    while (done < len) {
        ssize_t n = recv(fd, p + done, len - done, 0);
        if (n == 0) return -1;
        if (n < 0) { if (errno == EINTR) continue; return -1; }
        done += (size_t)n;
    }
    return 0;
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
        listen(server, 1) != 0 ||
        getsockname(server, (struct sockaddr *)&addr, &len) != 0) return 1;

    child = fork();
    if (child < 0) return 1;

    if (child == 0) {
        int client = socket(AF_INET, SOCK_STREAM, 0);
        uint32_t netlen = htonl(21U);
        if (client < 0) _exit(2);
        if (connect(client, (struct sockaddr *)&addr, sizeof addr) != 0) _exit(3);
        if (write_all(client, &netlen, sizeof netlen) != 0) _exit(4);
        if (write_all(client, "framed-message-works!", 21U) != 0) _exit(5);
        close(client);
        _exit(0);
    }

    {
        int peer = accept(server, NULL, NULL);
        uint32_t netlen = 0U;
        uint32_t msglen;
        char buf[128];
        if (peer < 0) return 1;
        if (read_exact(peer, &netlen, sizeof netlen) != 0) return 1;
        msglen = ntohl(netlen);
        if (msglen >= sizeof buf) return 1;
        if (read_exact(peer, buf, msglen) != 0) return 1;
        buf[msglen] = '\0';
        printf("decoded_length=%u\n", msglen);
        printf("decoded_message=%s\n", buf);
        printf("framing_verified=%s\n",
               msglen == 21U && strcmp(buf, "framed-message-works!") == 0 ? "yes" : "no");
        close(peer);
    }

    close(server);
    if (waitpid(child, &status, 0) < 0) return 1;
    return WIFEXITED(status) && WEXITSTATUS(status) == 0 ? 0 : 1;
}
