#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double seconds_between(struct timespec start, struct timespec end)
{
    const time_t sec = end.tv_sec - start.tv_sec;
    const long nsec = end.tv_nsec - start.tv_nsec;
    return (double)sec + (double)nsec / 1000000000.0;
}

__attribute__((noinline))
static unsigned char *allocate_buffer(size_t bytes)
{
    return malloc(bytes);
}

__attribute__((noinline))
static void release_buffer(void *ptr)
{
    free(ptr);
}

static uint64_t repeated_allocations(size_t iterations, size_t bytes)
{
    uint64_t sink = 0U;

    for (size_t i = 0U; i < iterations; ++i) {
        unsigned char *buffer = allocate_buffer(bytes);

        if (buffer == NULL) {
            return UINT64_MAX;
        }

        const size_t index = i % bytes;
        buffer[index] = (unsigned char)(i & 255U);
        sink += buffer[index];

        release_buffer(buffer);
    }

    return sink;
}

static uint64_t reused_allocation(size_t iterations, size_t bytes)
{
    unsigned char *buffer = allocate_buffer(bytes);

    if (buffer == NULL) {
        return UINT64_MAX;
    }

    uint64_t sink = 0U;

    for (size_t i = 0U; i < iterations; ++i) {
        const size_t index = i % bytes;
        buffer[index] = (unsigned char)(i & 255U);
        sink += buffer[index];
    }

    release_buffer(buffer);
    return sink;
}

int main(void)
{
    const size_t iterations = 500000U;
    const size_t bytes = 1024U;

    struct timespec a0;
    struct timespec a1;
    struct timespec b0;
    struct timespec b1;

    if (clock_gettime(CLOCK_MONOTONIC, &a0) != 0) {
        return 1;
    }

    const uint64_t repeated_sink =
        repeated_allocations(iterations, bytes);

    if (clock_gettime(CLOCK_MONOTONIC, &a1) != 0) {
        return 1;
    }

    if (clock_gettime(CLOCK_MONOTONIC, &b0) != 0) {
        return 1;
    }

    const uint64_t reused_sink =
        reused_allocation(iterations, bytes);

    if (clock_gettime(CLOCK_MONOTONIC, &b1) != 0) {
        return 1;
    }

    if (repeated_sink == UINT64_MAX ||
        reused_sink == UINT64_MAX) {
        return 1;
    }

    const double repeated_seconds =
        seconds_between(a0, a1);

    const double reused_seconds =
        seconds_between(b0, b1);

    printf("iterations=%zu\n", iterations);
    printf("repeated_alloc_seconds=%.9f\n",
           repeated_seconds);
    printf("reused_alloc_seconds=%.9f\n",
           reused_seconds);
    printf("sink_equal=%s\n",
           repeated_sink == reused_sink ? "yes" : "no");

    printf("allocation_reuse_ratio=%.3f\n",
           reused_seconds > 0.0
               ? repeated_seconds / reused_seconds
               : 0.0);

    return repeated_sink == reused_sink ? 0 : 1;
}
