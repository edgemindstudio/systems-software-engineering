#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int fd;
    struct stat info;
    const char prefix[] = "BEGIN\n";
    const char suffix[] = "END\n";
    const off_t hole_offset = 1024 * 1024;

    if (argc != 2) {
        fprintf(stderr, "usage: %s FILE\n", argv[0]);
        return 2;
    }

    fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        fprintf(stderr, "open: %s\n", strerror(errno));
        return 1;
    }

    if (write(fd, prefix, sizeof prefix - 1U) != (ssize_t)(sizeof prefix - 1U)) {
        perror("write prefix");
        return 1;
    }

    if (lseek(fd, hole_offset, SEEK_SET) < 0) {
        perror("lseek");
        return 1;
    }

    if (write(fd, suffix, sizeof suffix - 1U) != (ssize_t)(sizeof suffix - 1U)) {
        perror("write suffix");
        return 1;
    }

    if (fstat(fd, &info) != 0) {
        perror("fstat");
        return 1;
    }

    printf("logical_size=%lld\n", (long long)info.st_size);
    printf("allocated_512_blocks=%lld\n", (long long)info.st_blocks);
    printf("allocated_bytes=%lld\n", (long long)info.st_blocks * 512LL);

#ifdef SEEK_DATA
    {
        off_t data = lseek(fd, 0, SEEK_DATA);
        off_t hole = lseek(fd, 0, SEEK_HOLE);

        if (data >= 0) {
            printf("first_data=%lld\n", (long long)data);
        } else {
            printf("first_data=unsupported_or_error\n");
        }

        if (hole >= 0) {
            printf("first_hole=%lld\n", (long long)hole);
        } else {
            printf("first_hole=unsupported_or_error\n");
        }
    }
#endif

    (void)close(fd);
    return 0;
}
