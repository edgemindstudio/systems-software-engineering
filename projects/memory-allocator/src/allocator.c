#include "allocator/allocator.h"

#include <stdint.h>
#include <string.h>
#include <stdalign.h>

#define ALLOCATOR_MAGIC UINT64_C(0xA110CA70A110CA70)
#define BLOCK_MAGIC UINT64_C(0xB10CB10CB10CB10C)

typedef struct block_header block_header_t;

struct block_header {
    uint64_t magic;
    size_t size;
    int is_free;
    block_header_t *prev;
    block_header_t *next;
};

struct allocator {
    uint64_t magic;
    unsigned char *arena_begin;
    size_t arena_bytes;
    block_header_t *head;
};

static size_t alignment(void)
{
    return alignof(max_align_t);
}

static size_t align_up(size_t value)
{
    const size_t a = alignment();
    const size_t rem = value % a;
    return rem == 0U ? value : value + (a - rem);
}

static size_t header_size(void)
{
    return align_up(sizeof(block_header_t));
}

static unsigned char *payload_from_block(block_header_t *block)
{
    return (unsigned char *)block + header_size();
}

static const unsigned char *payload_from_block_const(const block_header_t *block)
{
    return (const unsigned char *)block + header_size();
}

static block_header_t *block_from_payload(void *ptr)
{
    return (block_header_t *)((unsigned char *)ptr - header_size());
}

static int pointer_in_arena(const allocator_t *allocator, const void *ptr)
{
    const unsigned char *p = ptr;
    const unsigned char *begin = allocator->arena_begin;
    const unsigned char *end = begin + allocator->arena_bytes;
    return p >= begin && p < end;
}

static void split_block(block_header_t *block, size_t wanted)
{
    const size_t hs = header_size();
    const size_t minimum_payload = alignment();

    if (block->size < wanted + hs + minimum_payload) {
        return;
    }

    unsigned char *new_address = payload_from_block(block) + wanted;
    block_header_t *remainder = (block_header_t *)new_address;

    remainder->magic = BLOCK_MAGIC;
    remainder->size = block->size - wanted - hs;
    remainder->is_free = 1;
    remainder->prev = block;
    remainder->next = block->next;

    if (remainder->next != NULL) {
        remainder->next->prev = remainder;
    }

    block->next = remainder;
    block->size = wanted;
}

static void merge_with_next(block_header_t *block)
{
    block_header_t *next = block->next;
    if (next == NULL || next->is_free == 0) {
        return;
    }

    block->size += header_size() + next->size;
    block->next = next->next;

    if (block->next != NULL) {
        block->next->prev = block;
    }
}

size_t allocator_control_size(void)
{
    return align_up(sizeof(allocator_t));
}

allocator_t *allocator_init(void *memory, size_t bytes)
{
    if (memory == NULL) {
        return NULL;
    }

    const uintptr_t raw = (uintptr_t)memory;
    const size_t a = alignment();
    const uintptr_t aligned = (raw + (uintptr_t)(a - 1U)) & ~(uintptr_t)(a - 1U);
    const size_t prefix = (size_t)(aligned - raw);

    if (bytes <= prefix) {
        return NULL;
    }

    bytes -= prefix;
    unsigned char *base = (unsigned char *)aligned;

    const size_t control = allocator_control_size();
    const size_t hs = header_size();
    if (bytes < control + hs + a) {
        return NULL;
    }

    allocator_t *allocator = (allocator_t *)base;
    allocator->magic = ALLOCATOR_MAGIC;
    allocator->arena_begin = base;
    allocator->arena_bytes = bytes;

    block_header_t *first = (block_header_t *)(base + control);
    first->magic = BLOCK_MAGIC;
    first->size = bytes - control - hs;
    first->is_free = 1;
    first->prev = NULL;
    first->next = NULL;
    allocator->head = first;

    return allocator;
}

void *allocator_malloc(allocator_t *allocator, size_t bytes)
{
    if (allocator == NULL || allocator->magic != ALLOCATOR_MAGIC || bytes == 0U) {
        return NULL;
    }

    if (bytes > SIZE_MAX - alignment()) {
        return NULL;
    }

    const size_t wanted = align_up(bytes);

    for (block_header_t *block = allocator->head; block != NULL; block = block->next) {
        if (block->magic == BLOCK_MAGIC && block->is_free != 0 && block->size >= wanted) {
            split_block(block, wanted);
            block->is_free = 0;
            return payload_from_block(block);
        }
    }

    return NULL;
}

void allocator_free(allocator_t *allocator, void *ptr)
{
    if (allocator == NULL || allocator->magic != ALLOCATOR_MAGIC || ptr == NULL) {
        return;
    }

    if (!pointer_in_arena(allocator, ptr)) {
        return;
    }

    block_header_t *block = block_from_payload(ptr);
    if (!pointer_in_arena(allocator, block) || block->magic != BLOCK_MAGIC) {
        return;
    }

    block->is_free = 1;

    merge_with_next(block);

    if (block->prev != NULL && block->prev->is_free != 0) {
        block = block->prev;
        merge_with_next(block);
    }
}

void *allocator_calloc(allocator_t *allocator, size_t count, size_t bytes)
{
    if (count != 0U && bytes > SIZE_MAX / count) {
        return NULL;
    }

    const size_t total = count * bytes;
    void *ptr = allocator_malloc(allocator, total);

    if (ptr != NULL) {
        (void)memset(ptr, 0, total);
    }

    return ptr;
}

void *allocator_realloc(allocator_t *allocator, void *ptr, size_t bytes)
{
    if (ptr == NULL) {
        return allocator_malloc(allocator, bytes);
    }

    if (bytes == 0U) {
        allocator_free(allocator, ptr);
        return NULL;
    }

    if (allocator == NULL || allocator->magic != ALLOCATOR_MAGIC ||
        !pointer_in_arena(allocator, ptr)) {
        return NULL;
    }

    block_header_t *block = block_from_payload(ptr);
    if (!pointer_in_arena(allocator, block) ||
        block->magic != BLOCK_MAGIC ||
        block->is_free != 0) {
        return NULL;
    }

    const size_t wanted = align_up(bytes);

    if (block->size >= wanted) {
        split_block(block, wanted);

        if (block->next != NULL &&
            block->next->is_free != 0 &&
            block->next->next != NULL &&
            block->next->next->is_free != 0) {
            merge_with_next(block->next);
        }

        return ptr;
    }

    if (block->next != NULL &&
        block->next->is_free != 0 &&
        block->size + header_size() + block->next->size >= wanted) {
        merge_with_next(block);
        split_block(block, wanted);
        block->is_free = 0;
        return ptr;
    }

    void *replacement = allocator_malloc(allocator, bytes);
    if (replacement == NULL) {
        return NULL;
    }

    const size_t copy_size = block->size < bytes ? block->size : bytes;
    (void)memcpy(replacement, ptr, copy_size);
    allocator_free(allocator, ptr);
    return replacement;
}

int allocator_validate(const allocator_t *allocator)
{
    if (allocator == NULL || allocator->magic != ALLOCATOR_MAGIC ||
        allocator->head == NULL) {
        return 0;
    }

    const unsigned char *arena_end = allocator->arena_begin + allocator->arena_bytes;
    const block_header_t *previous = NULL;

    for (const block_header_t *block = allocator->head;
         block != NULL;
         block = block->next) {
        if (block->magic != BLOCK_MAGIC) {
            return 0;
        }
        if (!pointer_in_arena(allocator, block)) {
            return 0;
        }
        if (block->prev != previous) {
            return 0;
        }

        const unsigned char *payload = payload_from_block_const(block);
        if (payload > arena_end || block->size > (size_t)(arena_end - payload)) {
            return 0;
        }

        if (block->next != NULL) {
            const unsigned char *expected_next = payload + block->size;
            if ((const unsigned char *)block->next != expected_next) {
                return 0;
            }
            if (block->is_free != 0 && block->next->is_free != 0) {
                return 0;
            }
        }

        previous = block;
    }

    return 1;
}

allocator_stats_t allocator_get_stats(const allocator_t *allocator)
{
    allocator_stats_t stats = {0};

    if (allocator == NULL || allocator->magic != ALLOCATOR_MAGIC) {
        return stats;
    }

    stats.arena_bytes = allocator->arena_bytes;

    for (const block_header_t *block = allocator->head;
         block != NULL;
         block = block->next) {
        ++stats.block_count;
        stats.payload_bytes += block->size;

        if (block->is_free != 0) {
            ++stats.free_blocks;
            stats.free_bytes += block->size;
            if (block->size > stats.largest_free_block) {
                stats.largest_free_block = block->size;
            }
        } else {
            ++stats.allocated_blocks;
            stats.allocated_bytes += block->size;
        }
    }

    return stats;
}
