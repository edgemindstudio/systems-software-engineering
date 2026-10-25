#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

static int count_open_fds(void)
{
    int count = 0;
    for (int fd = 0; fd < 256; ++fd) {
        if (fcntl(fd, F_GETFD) != -1) {
            ++count;
        }
    }
    return count;
}

int main(void)
{
    const int before = count_open_fds();
    int leaked[8];

    for (int i = 0; i < 8; ++i) {
        leaked[i] = open("/dev/null", O_RDONLY);
        if (leaked[i] < 0) return 1;
    }

    const int after_open = count_open_fds();

    for (int i = 0; i < 8; ++i) {
        if (close(leaked[i]) != 0) return 1;
    }

    const int after_close = count_open_fds();

    printf("fds_before=%d\n", before);
    printf("fds_after_open=%d\n", after_open);
    printf("fds_after_close=%d\n", after_close);
    printf("leak_reproduced=%s\n", after_open == before + 8 ? "yes" : "no");
    printf("cleanup_verified=%s\n", after_close == before ? "yes" : "no");
    return 0;
}
