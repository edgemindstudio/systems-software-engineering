# Memory Allocator Architecture

## Arena Model

The caller provides a contiguous byte region. `allocator_init()` aligns the beginning of that region and places allocator control metadata at the front.

The remaining arena is represented initially as one free block.

## Block Layout

Conceptually:

```text
+-------------------+
| allocator control |
+-------------------+
| block header      |
+-------------------+
| payload           |
+-------------------+
| block header      |
+-------------------+
| payload           |
+-------------------+
```

Block headers form a doubly linked physical list.

Payload addresses are aligned to `alignof(max_align_t)`.

## Allocation

`allocator_malloc()`:

1. rounds the request upward to allocator alignment;
2. scans blocks using first fit;
3. splits a free block when the remainder can hold another header plus a minimally aligned payload;
4. marks the selected block allocated;
5. returns its payload address.

## Free and Coalescing

`allocator_free()` marks the block free, merges with its next free neighbor, then merges through the previous neighbor when possible.

The invariant validator rejects two adjacent free blocks, so successful coalescing is directly testable.

## calloc

`allocator_calloc()` checks multiplication overflow, allocates the combined size, and zeroes the requested byte count.

## realloc

`allocator_realloc()` supports:

- `NULL` as allocation;
- zero size as free;
- shrink-in-place with optional split;
- growth into an adjacent free block when possible;
- allocate/copy/free fallback otherwise.

## Validation

`allocator_validate()` checks:

- allocator and block magic values;
- block addresses within the arena;
- correct previous links;
- block payload bounds;
- physical adjacency between consecutive blocks;
- absence of adjacent uncoalesced free blocks.

## Statistics

The allocator reports:

- arena bytes;
- aggregate payload bytes;
- allocated bytes;
- free bytes;
- largest free block;
- total block count;
- allocated block count;
- free block count.

These values expose fragmentation and recovery behavior without exposing allocator internals to callers.

## Scope

The implementation is intentionally single-threaded and fixed-arena. It does not call `sbrk()`, grow through `mmap()`, or replace libc allocation.
