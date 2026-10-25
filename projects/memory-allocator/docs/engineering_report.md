# Memory Allocator — Engineering Report

## Objective

Build a fixed-arena memory allocator in C that demonstrates allocator internals including alignment, block metadata, first-fit placement, block splitting, coalescing, allocation reuse, `calloc`, `realloc`, invariant validation, and fragmentation/state statistics.

The allocator intentionally manages caller-provided memory rather than replacing the process-wide libc allocator.

## Development Environment

Validation was performed on:

```text
Debian clang version 21.1.8 (7+b1)
Linux 6.6.87.2-microsoft-standard-WSL2 x86_64 GNU/Linux
```

## Build Quality

The normal build compiled the allocator, functional tests, and benchmark using C17 with:

```text
-Wall
-Wextra
-Wpedantic
-Wconversion
-Wshadow
-Wstrict-prototypes
-Wmissing-prototypes
```

The validation run completed without compiler warnings.

## Functional Validation

The allocator test suite verified the following behaviors:

```text
alignment=PASS
split_reuse=PASS
coalescing=PASS
calloc_zeroing=PASS
realloc_preservation=PASS
exhaustion_recovery=PASS
stats=PASS
allocator_tests=PASS
```

### Alignment

Returned payload addresses satisfied `alignof(max_align_t)` across a range of allocation sizes.

### Splitting and Reuse

A freed block was reused by a later smaller allocation, verifying first-fit selection and block splitting.

### Coalescing

Adjacent free blocks were merged and subsequently satisfied a larger allocation that depended on the recovered contiguous space.

### calloc

Allocated memory was verified to contain zero-initialized elements.

### realloc

Data written before growth remained intact after `realloc`, and data remained valid after shrinking.

The implementation exercised:

- shrink in place;
- growth into adjacent free space when possible;
- allocate/copy/free fallback behavior.

### Exhaustion and Recovery

The allocator was driven to exhaustion with repeated allocations and then completely freed.

After recovery:

```text
allocated_blocks=0
free_blocks=1
```

This demonstrates successful coalescing of the arena back into one free region.

### Statistics

The statistics interface correctly reflected allocated/free block counts and free-space recovery across allocation and deallocation.

## Steady-State Benchmark

The benchmark executed:

```text
iterations=200000
elapsed_seconds=0.000601196
allocated_blocks=0
free_blocks=1
largest_free_block=4194224
allocator_valid=yes
```

The elapsed time is specific to the development environment and workload and is not presented as a comparison against libc or other production allocators.

The important steady-state result is structural:

```text
allocated_blocks=0
free_blocks=1
allocator_valid=yes
```

After 200,000 allocate/write/free iterations, the arena remained valid and fully recovered to a single free block.

## Allocation Model

The allocator uses a doubly linked physical block list.

Each block stores:

- metadata magic value;
- payload size;
- free/allocated state;
- previous physical block;
- next physical block.

Allocation uses first-fit search.

A sufficiently large free block is split when the remaining space can contain another block header and a minimally aligned payload.

## Free and Coalescing Model

`allocator_free()` marks a block free and coalesces adjacent free neighbors.

The invariant checker rejects adjacent free blocks that should have been merged, making coalescing correctness directly testable.

## calloc Behavior

`allocator_calloc()` checks multiplication overflow before allocation.

If successful, the requested byte range is zero-filled.

## realloc Behavior

`allocator_realloc()` supports:

- `NULL` pointer as allocation;
- zero size as free;
- shrink in place;
- growth into a following free block;
- allocation/copy/free fallback.

When shrinking creates a free remainder adjacent to another free block, those free regions are coalesced to preserve allocator invariants.

## Invariant Validation

`allocator_validate()` checks:

- allocator control magic;
- block metadata magic;
- block addresses within the arena;
- correct previous-block links;
- payload bounds;
- exact physical adjacency of consecutive blocks;
- absence of uncoalesced adjacent free blocks.

The benchmark and every functional scenario completed with:

```text
allocator_valid=yes
```

or an equivalent successful assertion.

## Sanitizer Validation

The allocator test binary was rebuilt with:

```text
-fsanitize=address,undefined
-fno-omit-frame-pointer
```

and executed with:

```text
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1
UBSAN_OPTIONS=halt_on_error=1
```

The complete allocator functional suite passed again:

```text
alignment=PASS
split_reuse=PASS
coalescing=PASS
calloc_zeroing=PASS
realloc_preservation=PASS
exhaustion_recovery=PASS
stats=PASS
allocator_tests=PASS
```

No AddressSanitizer or UndefinedBehaviorSanitizer failure interrupted the validation run.

## Engineering Interpretation

This project demonstrates several core systems-software engineering competencies:

1. **Memory layout reasoning** — allocator control state, headers, and payloads share one caller-provided arena.
2. **Alignment discipline** — payload addresses satisfy the platform's maximum standard alignment requirement.
3. **Fragmentation management** — block splitting and bidirectional coalescing recover reusable space.
4. **Ownership and lifetime semantics** — allocated blocks transition explicitly between allocated and free states.
5. **API edge cases** — zero-size behavior, overflow-aware `calloc`, and `realloc` semantics are handled deliberately.
6. **Invariant-driven debugging** — structural correctness is checked independently through `allocator_validate()`.
7. **Observable allocator state** — statistics expose block counts, free space, allocated space, and largest free region.
8. **Evidence-backed validation** — strict builds, functional tests, a repeated workload, ASan, and UBSan all support the result.

## Scope

This is a portfolio allocator, not a production general-purpose heap implementation.

It intentionally does not provide:

- thread safety;
- multiple arenas;
- size classes;
- per-thread caches;
- system-level arena growth through `mmap()` or `sbrk()`;
- guard pages;
- quarantine;
- hardened metadata;
- comprehensive arbitrary-invalid-pointer detection;
- drop-in replacement of libc `malloc()`.

The allocator owns only the caller-provided fixed arena.

## Result

**PASS**

The Memory Allocator successfully completed strict compilation, alignment and allocation tests, split/reuse validation, coalescing validation, `calloc` and `realloc` checks, exhaustion/recovery tests, statistics checks, a 200,000-iteration steady-state workload, and ASan/UBSan validation on the development environment.
