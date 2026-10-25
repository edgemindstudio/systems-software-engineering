#include "sysutil/commands.h"
#include "sysutil/common.h"

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int parse_limit(const char *text, uintmax_t *value)
{
    errno = 0;
    char *end = NULL;
    const uintmax_t parsed = strtoumax(text, &end, 10);

    if (errno != 0 || end == text || *end != '\0') {
        return -1;
    }

    *value = parsed;
    return 0;
}

static void print_line(uintmax_t offset,
                       const unsigned char *buffer,
                       size_t size)
{
    (void)printf("%08jx  ", offset);

    for (size_t i = 0U; i < 16U; ++i) {
        if (i < size) {
            (void)printf("%02x ", (unsigned int)buffer[i]);
        } else {
            (void)printf("   ");
        }

        if (i == 7U) {
            (void)printf(" ");
        }
    }

    (void)printf(" |");
    for (size_t i = 0U; i < size; ++i) {
        const unsigned char ch = buffer[i];
        (void)putchar(isprint((int)ch) ? (int)ch : '.');
    }
    (void)printf("|\n");
}

int sysutil_cmd_hex(int argc, char **argv)
{
    if (argc < 2 || argc > 3) {
        (void)fprintf(stderr, "usage: sysutil hex FILE [MAX_BYTES]\n");
        return 2;
    }

    uintmax_t limit = 256U;
    if (argc == 3 && parse_limit(argv[2], &limit) != 0) {
        (void)fprintf(stderr, "sysutil: invalid MAX_BYTES '%s'\n", argv[2]);
        return 2;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        return sysutil_error_errno("open", argv[1]);
    }

    unsigned char buffer[16];
    uintmax_t offset = 0U;
    int result = 0;

    while (offset < limit) {
        size_t wanted = sizeof buffer;
        const uintmax_t remaining = limit - offset;
        if (remaining < (uintmax_t)wanted) {
            wanted = (size_t)remaining;
        }

        ssize_t count;
        do {
            count = read(fd, buffer, wanted);
        } while (count < 0 && errno == EINTR);

        if (count < 0) {
            result = sysutil_error_errno("read", argv[1]);
            break;
        }
        if (count == 0) {
            break;
        }

        print_line(offset, buffer, (size_t)count);
        offset += (uintmax_t)count;
    }

    if (close(fd) != 0 && result == 0) {
        result = sysutil_error_errno("close", argv[1]);
    }

    if (result == 0) {
        (void)printf("displayed_bytes=%ju\n", offset);
    }

    return result;
}
