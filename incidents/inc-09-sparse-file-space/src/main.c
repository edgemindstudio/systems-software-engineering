#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
    const char *path = "workspace/sparse.bin";
    int fd = open(path, O_CREAT | O_TRUNC | O_WRONLY, 0600);
    if (fd < 0) return 1;

    if (lseek(fd, 64 * 1024 * 1024, SEEK_SET) == (off_t)-1) return 1;
    if (write(fd, "X", 1) != 1) return 1;
    if (close(fd) != 0) return 1;

    struct stat st;
    if (stat(path, &st) != 0) return 1;

    const long long logical = (long long)st.st_size;
    const long long allocated = (long long)st.st_blocks * 512LL;

    printf("logical_bytes=%lld\n", logical);
    printf("allocated_bytes=%lld\n", allocated);
    printf("sparse_behavior_observed=%s\n", allocated < logical ? "yes" : "no");
    return allocated < logical ? 0 : 1;
}
