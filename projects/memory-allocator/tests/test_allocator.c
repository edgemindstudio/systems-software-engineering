#include "allocator/allocator.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdalign.h>

#define ARENA_SIZE (64U * 1024U)

static union {
    max_align_t alignment;
    unsigned char bytes[ARENA_SIZE];
} arena_storage;

static allocator_t *fresh_allocator(void)
{
    (void)memset(arena_storage.bytes, 0xA5, sizeof arena_storage.bytes);
    allocator_t *allocator = allocator_init(arena_storage.bytes, sizeof arena_storage.bytes);
    assert(allocator != NULL);
    assert(allocator_validate(allocator) == 1);
    return allocator;
}

static void test_alignment(void)
{
    allocator_t *a = fresh_allocator();

    for (size_t n = 1U; n <= 257U; ++n) {
        void *p = allocator_malloc(a, n);
        assert(p != NULL);
        assert((uintptr_t)p % alignof(max_align_t) == 0U);
    }

    assert(allocator_validate(a) == 1);
    puts("alignment=PASS");
}

static void test_split_and_reuse(void)
{
    allocator_t *a = fresh_allocator();

    void *first = allocator_malloc(a, 128U);
    void *second = allocator_malloc(a, 256U);
    assert(first != NULL && second != NULL && first != second);

    allocator_free(a, first);
    void *replacement = allocator_malloc(a, 64U);

    assert(replacement == first);
    assert(allocator_validate(a) == 1);
    puts("split_reuse=PASS");
}

static void test_coalescing(void)
{
    allocator_t *a = fresh_allocator();

    void *a1 = allocator_malloc(a, 1024U);
    void *a2 = allocator_malloc(a, 1024U);
    void *a3 = allocator_malloc(a, 1024U);
    assert(a1 != NULL && a2 != NULL && a3 != NULL);

    allocator_free(a, a2);
    allocator_free(a, a1);

    allocator_stats_t stats = allocator_get_stats(a);
    assert(stats.free_blocks >= 1U);
    assert(allocator_validate(a) == 1);

    void *large = allocator_malloc(a, 1800U);
    assert(large == a1);
    assert(allocator_validate(a) == 1);
    puts("coalescing=PASS");
}

static void test_calloc(void)
{
    allocator_t *a = fresh_allocator();

    uint32_t *values = allocator_calloc(a, 128U, sizeof *values);
    assert(values != NULL);

    for (size_t i = 0U; i < 128U; ++i) {
        assert(values[i] == 0U);
    }

    assert(allocator_validate(a) == 1);
    puts("calloc_zeroing=PASS");
}

static void test_realloc(void)
{
    allocator_t *a = fresh_allocator();

    unsigned char *p = allocator_malloc(a, 64U);
    assert(p != NULL);
    for (size_t i = 0U; i < 64U; ++i) {
        p[i] = (unsigned char)i;
    }

    unsigned char *grown = allocator_realloc(a, p, 512U);
    assert(grown != NULL);
    for (size_t i = 0U; i < 64U; ++i) {
        assert(grown[i] == (unsigned char)i);
    }

    unsigned char *shrunk = allocator_realloc(a, grown, 32U);
    assert(shrunk != NULL);
    for (size_t i = 0U; i < 32U; ++i) {
        assert(shrunk[i] == (unsigned char)i);
    }

    assert(allocator_validate(a) == 1);
    puts("realloc_preservation=PASS");
}

static void test_exhaustion_and_recovery(void)
{
    allocator_t *a = fresh_allocator();

    void *blocks[256];
    size_t count = 0U;

    while (count < 256U) {
        void *p = allocator_malloc(a, 1024U);
        if (p == NULL) {
            break;
        }
        blocks[count++] = p;
    }

    assert(count > 0U);
    assert(allocator_malloc(a, ARENA_SIZE) == NULL);

    for (size_t i = 0U; i < count; ++i) {
        allocator_free(a, blocks[i]);
    }

    allocator_stats_t stats = allocator_get_stats(a);
    assert(stats.allocated_blocks == 0U);
    assert(stats.free_blocks == 1U);
    assert(allocator_validate(a) == 1);
    puts("exhaustion_recovery=PASS");
}

static void test_stats(void)
{
    allocator_t *a = fresh_allocator();
    allocator_stats_t before = allocator_get_stats(a);

    void *p1 = allocator_malloc(a, 100U);
    void *p2 = allocator_malloc(a, 200U);
    assert(p1 != NULL && p2 != NULL);

    allocator_stats_t during = allocator_get_stats(a);
    assert(during.allocated_blocks == 2U);
    assert(during.allocated_bytes >= 300U);
    assert(during.free_bytes < before.free_bytes);

    allocator_free(a, p1);
    allocator_free(a, p2);

    allocator_stats_t after = allocator_get_stats(a);
    assert(after.allocated_blocks == 0U);
    assert(after.free_blocks == 1U);
    assert(after.free_bytes == before.free_bytes);

    puts("stats=PASS");
}

int main(void)
{
    test_alignment();
    test_split_and_reuse();
    test_coalescing();
    test_calloc();
    test_realloc();
    test_exhaustion_and_recovery();
    test_stats();

    puts("allocator_tests=PASS");
    return 0;
}
