#include "sysdiag/protocol.h"

#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

int sysdiag_read_exact(int fd, void *buffer, size_t bytes)
{
    unsigned char *cursor = buffer;
    size_t remaining = bytes;

    while (remaining > 0U) {
        ssize_t n = read(fd, cursor, remaining);

        if (n == 0) {
            return 0;
        }
        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }

        cursor += (size_t)n;
        remaining -= (size_t)n;
    }

    return 1;
}

int sysdiag_write_all(int fd, const void *buffer, size_t bytes)
{
    const unsigned char *cursor = buffer;
    size_t remaining = bytes;

    while (remaining > 0U) {
        ssize_t n = write(fd, cursor, remaining);

        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }

        cursor += (size_t)n;
        remaining -= (size_t)n;
    }

    return 0;
}

int sysdiag_read_frame(int fd,
                       unsigned char *buffer,
                       size_t capacity,
                       uint32_t *length_out)
{
    uint32_t network_length = 0U;

    int rc = sysdiag_read_exact(fd,
                                &network_length,
                                sizeof network_length);
    if (rc <= 0) {
        return rc;
    }

    uint32_t length = ntohl(network_length);

    if ((size_t)length > capacity) {
        errno = EMSGSIZE;
        return -1;
    }

    if (length > 0U) {
        rc = sysdiag_read_exact(fd, buffer, (size_t)length);
        if (rc <= 0) {
            return -1;
        }
    }

    *length_out = length;
    return 1;
}

int sysdiag_write_frame(int fd,
                        const unsigned char *buffer,
                        uint32_t length)
{
    uint32_t network_length = htonl(length);

    if (sysdiag_write_all(fd,
                          &network_length,
                          sizeof network_length) != 0) {
        return -1;
    }

    if (length > 0U &&
        sysdiag_write_all(fd, buffer, (size_t)length) != 0) {
        return -1;
    }

    return 0;
}
