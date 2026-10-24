#include <float.h>
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


static uint64_t workload(uint64_t iterations)
{
    uint64_t value = 1469598103934665603ULL;
    for (uint64_t i = 0U; i < iterations; ++i) {
        value ^= i + 0x9e3779b97f4a7c15ULL;
        value *= 1099511628211ULL;
    }
    return value;
}

int main(void)
{
    enum { trials = 7 };
    const uint64_t iterations = 5000000U;
    double minimum = DBL_MAX;
    double maximum = 0.0;
    double total = 0.0;
    uint64_t combined = 0U;

    for (int t = 0; t < trials; ++t) {
        struct timespec start;
        struct timespec end;
        if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) return 1;
        const uint64_t result = workload(iterations);
        if (clock_gettime(CLOCK_MONOTONIC, &end) != 0) return 1;

        const double elapsed = seconds_between(start, end);
        if (elapsed < minimum) minimum = elapsed;
        if (elapsed > maximum) maximum = elapsed;
        total += elapsed;
        combined ^= result;
        printf("trial_%d_seconds=%.9f\n", t + 1, elapsed);
    }

    const double mean = total / (double)trials;
    printf("min_seconds=%.9f\n", minimum);
    printf("mean_seconds=%.9f\n", mean);
    printf("max_seconds=%.9f\n", maximum);
    printf("combined_sink=%" PRIu64 "\n", combined);
    printf("statistics_order_valid=%s\n",
           minimum <= mean && mean <= maximum ? "yes" : "no");
    return minimum <= mean && mean <= maximum ? 0 : 1;
}
