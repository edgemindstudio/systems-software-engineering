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


static uint64_t compute(uint64_t iterations)
{
    uint64_t value = 0U;
    for (uint64_t i = 1U; i <= iterations; ++i) {
        value += (i * 2654435761U) ^ (i >> 5U);
    }
    return value;
}

int main(void)
{
    const uint64_t iterations = 80000000U;
    struct timespec start, end;
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) return 1;
    const uint64_t result = compute(iterations);
    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0) return 1;
    printf("iterations=%" PRIu64 "\n", iterations);
    printf("result=%" PRIu64 "\n", result);
    printf("elapsed_seconds=%.9f\n", seconds_between(start, end));
    return 0;
}
