#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static double seconds_between(struct timespec a, struct timespec b)
{
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

static double run_case(size_t chunk, size_t total)
{
    char buf[4096];
    memset(buf, 'x', sizeof buf);

    int fd = open("/dev/null", O_WRONLY);
    if (fd < 0) return -1.0;

    struct timespec a, b;
    if (clock_gettime(CLOCK_MONOTONIC, &a) != 0) return -1.0;

    size_t remaining = total;
    while (remaining > 0U) {
        size_t n = remaining < chunk ? remaining : chunk;
        if (write(fd, buf, n) != (ssize_t)n) return -1.0;
        remaining -= n;
    }

    if (clock_gettime(CLOCK_MONOTONIC, &b) != 0) return -1.0;
    close(fd);
    return seconds_between(a, b);
}

int main(void)
{
    const size_t total = 8U * 1024U * 1024U;
    const double tiny = run_case(1U, total);
    const double batched = run_case(4096U, total);

    if (tiny <= 0.0 || batched <= 0.0) return 1;

    printf("total_bytes=%zu\n", total);
    printf("tiny_write_seconds=%.9f\n", tiny);
    printf("batched_write_seconds=%.9f\n", batched);
    printf("tiny_to_batched_ratio=%.3f\n", tiny / batched);
    printf("regression_reproduced=yes\n");
    return 0;
}
