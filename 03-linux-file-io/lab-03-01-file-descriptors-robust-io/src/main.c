#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int write_all(int fd, const unsigned char *buffer, size_t length)
{
    size_t written_total = 0U;

    while (written_total < length) {
        ssize_t written = write(fd, buffer + written_total, length - written_total);

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

        written_total += (size_t)written;
    }

    return 0;
}

static int copy_fd(int source_fd, int destination_fd)
{
    unsigned char buffer[4096];

    for (;;) {
        ssize_t count = read(source_fd, buffer, sizeof buffer);

        if (count < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }

        if (count == 0) {
            return 0;
        }

        if (write_all(destination_fd, buffer, (size_t)count) != 0) {
            return -1;
        }
    }
}

int main(int argc, char **argv)
{
    int source_fd;
    int destination_fd;
    int status = 0;

    if (argc != 3) {
        fprintf(stderr, "usage: %s SOURCE DESTINATION\n", argv[0]);
        return 2;
    }

    source_fd = open(argv[1], O_RDONLY);
    if (source_fd < 0) {
        fprintf(stderr, "open source: %s\n", strerror(errno));
        return 1;
    }

    destination_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destination_fd < 0) {
        fprintf(stderr, "open destination: %s\n", strerror(errno));
        (void)close(source_fd);
        return 1;
    }

    if (copy_fd(source_fd, destination_fd) != 0) {
        fprintf(stderr, "copy: %s\n", strerror(errno));
        status = 1;
    }

    if (close(destination_fd) != 0) {
        fprintf(stderr, "close destination: %s\n", strerror(errno));
        status = 1;
    }

    if (close(source_fd) != 0) {
        fprintf(stderr, "close source: %s\n", strerror(errno));
        status = 1;
    }

    return status;
}
