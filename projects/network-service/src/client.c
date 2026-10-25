#include "netservice/protocol.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static int parse_port(const char *text, uint16_t *out)
{
    char *end = NULL;
    errno = 0;
    unsigned long value = strtoul(text, &end, 10);

    if (errno != 0 || end == text || *end != '\0' ||
        value == 0UL || value > 65535UL) {
        return -1;
    }

    *out = (uint16_t)value;
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        (void)fprintf(stderr,
                      "usage: netservice-client PORT MESSAGE\n");
        return 2;
    }

    uint16_t port = 0U;
    if (parse_port(argv[1], &port) != 0) {
        return 2;
    }

    size_t message_length = strlen(argv[2]);
    if (message_length > NETSERVICE_MAX_FRAME) {
        (void)fprintf(stderr, "message too large\n");
        return 2;
    }

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in address;
    (void)memset(&address, 0, sizeof address);
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1",
                  &address.sin_addr) != 1) {
        (void)close(fd);
        return 1;
    }

    if (connect(fd,
                (struct sockaddr *)&address,
                sizeof address) != 0) {
        perror("connect");
        (void)close(fd);
        return 1;
    }

    if (netservice_write_frame(fd,
                               (const unsigned char *)argv[2],
                               (uint32_t)message_length) != 0) {
        perror("write_frame");
        (void)close(fd);
        return 1;
    }

    unsigned char response[NETSERVICE_MAX_FRAME + 1U];
    uint32_t response_length = 0U;

    int rc = netservice_read_frame(fd,
                                   response,
                                   NETSERVICE_MAX_FRAME,
                                   &response_length);

    if (rc != 1) {
        perror("read_frame");
        (void)close(fd);
        return 1;
    }

    response[response_length] = '\0';

    (void)printf("response_length=%u\n",
                 (unsigned int)response_length);
    (void)printf("response=%s\n", response);

    (void)close(fd);
    return 0;
}
