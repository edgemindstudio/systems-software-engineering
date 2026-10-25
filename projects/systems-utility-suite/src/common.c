#include "sysutil/common.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int sysutil_error_errno(const char *operation, const char *path)
{
    if (path != NULL) {
        (void)fprintf(stderr, "sysutil: %s '%s': %s\n",
                      operation, path, strerror(errno));
    } else {
        (void)fprintf(stderr, "sysutil: %s: %s\n",
                      operation, strerror(errno));
    }
    return 1;
}

int sysutil_write_all(int fd, const void *buffer, size_t size)
{
    const unsigned char *cursor = buffer;
    size_t remaining = size;

    while (remaining > 0U) {
        const ssize_t written = write(fd, cursor, remaining);

        if (written < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }

        if (written == 0) {
            errno = EIO;
            return -1;
        }

        cursor += (size_t)written;
        remaining -= (size_t)written;
    }

    return 0;
}

const char *sysutil_file_type(mode_t mode)
{
    if (S_ISREG(mode)) return "regular";
    if (S_ISDIR(mode)) return "directory";
    if (S_ISLNK(mode)) return "symlink";
    if (S_ISCHR(mode)) return "character-device";
    if (S_ISBLK(mode)) return "block-device";
    if (S_ISFIFO(mode)) return "fifo";
    if (S_ISSOCK(mode)) return "socket";
    return "unknown";
}
