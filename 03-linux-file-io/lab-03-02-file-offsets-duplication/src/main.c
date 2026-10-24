#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int read_byte(int fd, char *out)
{
    ssize_t result;

    do {
        result = read(fd, out, 1U);
    } while (result < 0 && errno == EINTR);

    return result == 1 ? 0 : -1;
}

int main(int argc, char **argv)
{
    int fd1;
    int fd2;
    int fd3;
    char a;
    char b;
    char c;
    off_t off1;
    off_t off2;
    off_t off3;

    if (argc != 2) {
        fprintf(stderr, "usage: %s FILE\n", argv[0]);
        return 2;
    }

    fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0) {
        fprintf(stderr, "open: %s\n", strerror(errno));
        return 1;
    }

    fd2 = dup(fd1);
    if (fd2 < 0) {
        fprintf(stderr, "dup: %s\n", strerror(errno));
        (void)close(fd1);
        return 1;
    }

    fd3 = open(argv[1], O_RDONLY);
    if (fd3 < 0) {
        fprintf(stderr, "second open: %s\n", strerror(errno));
        (void)close(fd2);
        (void)close(fd1);
        return 1;
    }

    if (read_byte(fd1, &a) != 0 ||
        read_byte(fd2, &b) != 0 ||
        read_byte(fd3, &c) != 0) {
        fprintf(stderr, "read failed\n");
        return 1;
    }

    off1 = lseek(fd1, 0, SEEK_CUR);
    off2 = lseek(fd2, 0, SEEK_CUR);
    off3 = lseek(fd3, 0, SEEK_CUR);

    printf("fd1_byte=%c\n", a);
    printf("dup_fd2_byte=%c\n", b);
    printf("independent_fd3_byte=%c\n", c);
    printf("fd1_offset=%lld\n", (long long)off1);
    printf("fd2_offset=%lld\n", (long long)off2);
    printf("fd3_offset=%lld\n", (long long)off3);

    (void)close(fd3);
    (void)close(fd2);
    (void)close(fd1);
    return 0;
}
