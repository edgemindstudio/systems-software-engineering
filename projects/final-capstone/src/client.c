#include "sysdiag/protocol.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

static void usage(void)
{
    fprintf(stderr,
            "usage: sysdiag --socket PATH COMMAND [ARG]\n");
}

int main(int argc, char **argv)
{
    if (argc < 4 || strcmp(argv[1], "--socket") != 0) {
        usage();
        return 2;
    }

    const char *socket_path = argv[2];
    if (strlen(socket_path) >= sizeof(((struct sockaddr_un *)0)->sun_path)) {
        return 2;
    }

    char request[SYSDIAG_MAX_REQUEST + 1U];
    size_t used = 0U;

    for (int i = 3; i < argc; ++i) {
        if (i > 3) {
            if (used + 1U >= sizeof request) {
                return 2;
            }
            request[used++] = ' ';
        }

        size_t part = strlen(argv[i]);
        if (part > sizeof request - used - 1U) {
            return 2;
        }
        (void)memcpy(request + used, argv[i], part);
        used += part;
    }
    request[used] = '\0';

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_un address;
    (void)memset(&address, 0, sizeof address);
    address.sun_family = AF_UNIX;
    (void)snprintf(address.sun_path,
                   sizeof address.sun_path,
                   "%s",
                   socket_path);

    if (connect(fd,
                (struct sockaddr *)&address,
                sizeof address) != 0) {
        perror("connect");
        (void)close(fd);
        return 1;
    }

    if (sysdiag_write_frame(fd,
                            (const unsigned char *)request,
                            (uint32_t)used) != 0) {
        perror("write");
        (void)close(fd);
        return 1;
    }

    unsigned char response[SYSDIAG_MAX_RESPONSE + 1U];
    uint32_t response_length = 0U;

    int rc = sysdiag_read_frame(fd,
                                response,
                                SYSDIAG_MAX_RESPONSE,
                                &response_length);
    if (rc != 1) {
        perror("read");
        (void)close(fd);
        return 1;
    }

    response[response_length] = '\0';
    printf("%s", response);

    (void)close(fd);
    return strncmp((const char *)response, "OK ", 3U) == 0 ? 0 : 1;
}
