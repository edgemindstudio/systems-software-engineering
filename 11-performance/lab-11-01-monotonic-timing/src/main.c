#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>


static double seconds_between(struct timespec start, struct timespec end)
{
    const time_t sec = end.tv_sec - start.tv_sec;
    const long nsec = end.tv_nsec - start.tv_nsec;
    return (double)sec + (double)nsec / 1000000000.0;
}


int main(void)
{
    struct timespec start;
    struct timespec end;
    volatile uint64_t sink = 0U;
    const uint64_t iterations = 20000000U;

    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
        return 1;
    }

    for (uint64_t i = 0U; i < iterations; ++i) {
        sink += (i ^ (i >> 3U));
    }

    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0) {
        return 1;
    }

    const double elapsed = seconds_between(start, end);
    printf("iterations=%" PRIu64 "\n", iterations);
    printf("sink=%" PRIu64 "\n", (uint64_t)sink);
    printf("elapsed_seconds=%.9f\n", elapsed);
    printf("monotonic_elapsed_positive=%s\n", elapsed > 0.0 ? "yes" : "no");
    return elapsed > 0.0 ? 0 : 1;
}
