#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    const char header[] = "TYPE=event\n";
    const char body[] = "payload=systems-io\n";
    const char footer[] = "END\n";
    struct iovec output[3];
    char part1[sizeof header] = {0};
    char part2[sizeof body] = {0};
    char part3[sizeof footer] = {0};
    struct iovec input[3];
    int fd;
    ssize_t written;
    ssize_t read_count;

    if (argc != 2) {
        fprintf(stderr, "usage: %s FILE\n", argv[0]);
        return 2;
    }

    fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    output[0].iov_base = (void *)header;
    output[0].iov_len = sizeof header - 1U;
    output[1].iov_base = (void *)body;
    output[1].iov_len = sizeof body - 1U;
    output[2].iov_base = (void *)footer;
    output[2].iov_len = sizeof footer - 1U;

    written = writev(fd, output, 3);
    if (written < 0) {
        perror("writev");
        return 1;
    }

    if (lseek(fd, 0, SEEK_SET) < 0) {
        perror("lseek");
        return 1;
    }

    input[0].iov_base = part1;
    input[0].iov_len = sizeof header - 1U;
    input[1].iov_base = part2;
    input[1].iov_len = sizeof body - 1U;
    input[2].iov_base = part3;
    input[2].iov_len = sizeof footer - 1U;

    read_count = readv(fd, input, 3);
    if (read_count < 0) {
        perror("readv");
        return 1;
    }

    printf("writev_bytes=%zd\n", written);
    printf("readv_bytes=%zd\n", read_count);
    printf("part1=%s", part1);
    printf("part2=%s", part2);
    printf("part3=%s", part3);

    (void)close(fd);
    return 0;
}
