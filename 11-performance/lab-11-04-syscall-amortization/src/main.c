#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


static double seconds_between(struct timespec start, struct timespec end)
{
    const time_t sec = end.tv_sec - start.tv_sec;
    const long nsec = end.tv_nsec - start.tv_nsec;
    return (double)sec + (double)nsec / 1000000000.0;
}


static int write_all(int fd, const char *buf, size_t size)
{
    size_t done = 0U;
    while (done < size) {
        const ssize_t rc = write(fd, buf + done, size - done);
        if (rc <= 0) return -1;
        done += (size_t)rc;
    }
    return 0;
}

static double run_case(size_t chunk_size, size_t total_bytes)
{
    char buffer[4096];
    memset(buffer, 'x', sizeof buffer);
    const int fd = open("/dev/null", O_WRONLY);
    if (fd < 0) return -1.0;

    struct timespec start, end;
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
        close(fd);
        return -1.0;
    }

    size_t remaining = total_bytes;
    while (remaining > 0U) {
        const size_t amount = remaining < chunk_size ? remaining : chunk_size;
        if (write_all(fd, buffer, amount) != 0) {
            close(fd);
            return -1.0;
        }
        remaining -= amount;
    }

    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0) {
        close(fd);
        return -1.0;
    }
    close(fd);
    return seconds_between(start, end);
}

int main(void)
{
    const size_t total = 32U * 1024U * 1024U;
    const double one_byte = run_case(1U, total);
    const double four_k = run_case(4096U, total);
    if (one_byte <= 0.0 || four_k <= 0.0) return 1;

    printf("total_bytes=%zu\n", total);
    printf("one_byte_seconds=%.9f\n", one_byte);
    printf("four_kib_seconds=%.9f\n", four_k);
    printf("small_to_large_ratio=%.3f\n", one_byte / four_k);
    printf("same_total_bytes=yes\n");
    return 0;
}
