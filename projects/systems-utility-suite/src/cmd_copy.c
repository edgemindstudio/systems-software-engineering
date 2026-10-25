#include "sysutil/commands.h"
#include "sysutil/common.h"

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int sysutil_cmd_copy(int argc, char **argv)
{
    if (argc != 3) {
        (void)fprintf(stderr, "usage: sysutil copy SOURCE DESTINATION\n");
        return 2;
    }

    const char *source = argv[1];
    const char *destination = argv[2];

    int source_fd = open(source, O_RDONLY);
    if (source_fd < 0) {
        return sysutil_error_errno("open", source);
    }

    struct stat info;
    if (fstat(source_fd, &info) != 0) {
        const int saved = errno;
        (void)close(source_fd);
        errno = saved;
        return sysutil_error_errno("fstat", source);
    }

    if (!S_ISREG(info.st_mode)) {
        (void)fprintf(stderr, "sysutil: copy source is not a regular file: '%s'\n",
                      source);
        (void)close(source_fd);
        return 1;
    }

    int destination_fd = open(destination,
                              O_WRONLY | O_CREAT | O_TRUNC,
                              info.st_mode & 0777);
    if (destination_fd < 0) {
        const int saved = errno;
        (void)close(source_fd);
        errno = saved;
        return sysutil_error_errno("open", destination);
    }

    unsigned char buffer[16384];
    uintmax_t total = 0U;
    int result = 0;

    for (;;) {
        const ssize_t count = read(source_fd, buffer, sizeof buffer);

        if (count < 0) {
            if (errno == EINTR) {
                continue;
            }
            result = sysutil_error_errno("read", source);
            break;
        }

        if (count == 0) {
            break;
        }

        if (sysutil_write_all(destination_fd, buffer, (size_t)count) != 0) {
            result = sysutil_error_errno("write", destination);
            break;
        }

        total += (uintmax_t)count;
    }

    if (result == 0 &&
        fchmod(destination_fd, info.st_mode & 0777) != 0) {
        result = sysutil_error_errno("fchmod", destination);
    }

    if (close(source_fd) != 0 && result == 0) {
        result = sysutil_error_errno("close", source);
    }

    if (close(destination_fd) != 0 && result == 0) {
        result = sysutil_error_errno("close", destination);
    }

    if (result == 0) {
        (void)printf("copied_bytes=%ju\n", total);
    }

    return result;
}
