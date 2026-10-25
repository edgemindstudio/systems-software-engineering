#ifndef ALLOCATOR_ALLOCATOR_H
#define ALLOCATOR_ALLOCATOR_H

#include <stddef.h>

typedef struct allocator allocator_t;

typedef struct {
    size_t arena_bytes;
    size_t payload_bytes;
    size_t allocated_bytes;
    size_t free_bytes;
    size_t largest_free_block;
    size_t block_count;
    size_t allocated_blocks;
    size_t free_blocks;
} allocator_stats_t;

size_t allocator_control_size(void);

allocator_t *allocator_init(void *memory, size_t bytes);

void *allocator_malloc(allocator_t *allocator, size_t bytes);
void *allocator_calloc(allocator_t *allocator, size_t count, size_t bytes);
void *allocator_realloc(allocator_t *allocator, void *ptr, size_t bytes);
void allocator_free(allocator_t *allocator, void *ptr);

int allocator_validate(const allocator_t *allocator);
allocator_stats_t allocator_get_stats(const allocator_t *allocator);

#endif
