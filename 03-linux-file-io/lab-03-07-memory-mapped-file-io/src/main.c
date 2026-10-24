#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int fd;
    struct stat info;
    unsigned char *mapping;

    if (argc != 2) {
        fprintf(stderr, "usage: %s FILE\n", argv[0]);
        return 2;
    }

    fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    if (fstat(fd, &info) != 0) {
        perror("fstat");
        return 1;
    }

    if (info.st_size < 5) {
        fprintf(stderr, "file too small\n");
        return 1;
    }

    mapping = mmap(NULL, (size_t)info.st_size,
                   PROT_READ | PROT_WRITE,
                   MAP_SHARED, fd, 0);
    if (mapping == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    memcpy(mapping, "MMAP!", 5U);

    if (msync(mapping, (size_t)info.st_size, MS_SYNC) != 0) {
        perror("msync");
        return 1;
    }

    printf("mapped_size=%lld\n", (long long)info.st_size);
    printf("new_prefix=%.5s\n", mapping);

    if (munmap(mapping, (size_t)info.st_size) != 0) {
        perror("munmap");
        return 1;
    }

    (void)close(fd);
    return 0;
}
