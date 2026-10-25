#include "allocator/allocator.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdalign.h>

#define ARENA_SIZE (4U * 1024U * 1024U)
#define ITERATIONS 200000U

static union {
    max_align_t alignment;
    unsigned char bytes[ARENA_SIZE];
} arena_storage;

static double seconds_between(struct timespec start, struct timespec end)
{
    return (double)(end.tv_sec - start.tv_sec) +
           (double)(end.tv_nsec - start.tv_nsec) / 1000000000.0;
}

int main(void)
{
    allocator_t *allocator =
        allocator_init(arena_storage.bytes, sizeof arena_storage.bytes);

    if (allocator == NULL) {
        return 1;
    }

    struct timespec start;
    struct timespec end;

    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
        return 1;
    }

    for (size_t i = 0U; i < ITERATIONS; ++i) {
        void *p = allocator_malloc(allocator, 64U + (i % 128U));
        if (p == NULL) {
            return 1;
        }
        (void)memset(p, (int)(i & 0xFFU), 64U);
        allocator_free(allocator, p);
    }

    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0) {
        return 1;
    }

    allocator_stats_t stats = allocator_get_stats(allocator);

    printf("iterations=%u\n", (unsigned int)ITERATIONS);
    printf("elapsed_seconds=%.9f\n", seconds_between(start, end));
    printf("allocated_blocks=%zu\n", stats.allocated_blocks);
    printf("free_blocks=%zu\n", stats.free_blocks);
    printf("largest_free_block=%zu\n", stats.largest_free_block);
    printf("allocator_valid=%s\n",
           allocator_validate(allocator) ? "yes" : "no");

    return allocator_validate(allocator) ? 0 : 1;
}
