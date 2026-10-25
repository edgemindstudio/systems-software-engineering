# Memory Allocator

A fixed-arena memory allocator implemented in C.

The project focuses on allocator internals rather than replacing libc globally. A caller supplies an arena, and the allocator manages blocks inside that region.

## Features

- `max_align_t`-compatible payload alignment
- first-fit allocation
- block splitting
- adjacent free-block coalescing
- allocation reuse
- `malloc`-style allocation
- `calloc`-style zero initialization with overflow checking
- `realloc`-style growth/shrink behavior
- allocator invariant validation
- allocation/free-space statistics
- strict-warning build
- ASan/UBSan validation
- steady-state benchmark

## Public API

```c
allocator_t *allocator_init(void *memory, size_t bytes);

void *allocator_malloc(allocator_t *allocator, size_t bytes);
void *allocator_calloc(allocator_t *allocator, size_t count, size_t bytes);
void *allocator_realloc(allocator_t *allocator, void *ptr, size_t bytes);
void allocator_free(allocator_t *allocator, void *ptr);

int allocator_validate(const allocator_t *allocator);
allocator_stats_t allocator_get_stats(const allocator_t *allocator);
```

## Design

The arena begins with allocator control state followed by a doubly linked physical block list.

Each block records:

- payload size
- free/allocated state
- previous physical block
- next physical block
- metadata magic value

Allocation uses first fit. A sufficiently large free block is split, leaving a reusable remainder.

Freeing coalesces with adjacent free blocks so fragmentation can recover after objects are released.

## Build

```bash
make
```

## Tests

```bash
make test
```

The test suite checks alignment, splitting/reuse, coalescing, zero initialization, `realloc` preservation, exhaustion/recovery, statistics, and allocator invariants.

## Sanitizers

```bash
make sanitize
```

## Benchmark

```bash
make benchmark
```

The benchmark performs a repeated allocate/write/free workload and reports elapsed time plus final allocator state. Timing is environment-specific and is not presented as a comparison with libc.

## Full Validation

```bash
./scripts/validate.sh
```

The resulting transcript is written to:

```text
evidence/validation.txt
```

## Scope and Safety

This is an educational/portfolio allocator and does not replace the process-wide `malloc()` implementation.

It is intentionally single-threaded and does not provide:

- thread safety
- guard pages
- quarantine
- hardened metadata
- size classes
- per-thread caches
- system-level arena growth
- production-grade invalid-free detection

The allocator silently ignores pointers that are clearly outside the managed arena, but arbitrary invalid interior pointers are outside the supported API contract.
