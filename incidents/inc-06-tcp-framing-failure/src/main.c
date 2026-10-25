#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void)
{
    int sv[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) != 0) return 1;

    const char *a = "HELLO";
    const char *b = "WORLD";

    if (write(sv[0], a, strlen(a)) != 5) return 1;
    if (write(sv[0], b, strlen(b)) != 5) return 1;
    if (shutdown(sv[0], SHUT_WR) != 0) return 1;

    char buffer[32] = {0};
    size_t total = 0U;

    for (;;) {
        ssize_t n = read(sv[1], buffer + total, sizeof buffer - 1U - total);
        if (n < 0) return 1;
        if (n == 0) break;
        total += (size_t)n;
    }

    printf("bytes_received=%zu\n", total);
    printf("stream_payload=%s\n", buffer);
    printf("message_boundaries_preserved=no\n");
    printf("framing_required=yes\n");

    close(sv[0]);
    close(sv[1]);
    return strcmp(buffer, "HELLOWORLD") == 0 ? 0 : 1;
}
