# Dynamic Memory — Engineering Report

## Objective

Implement explicit heap ownership, checked allocation, dynamic growth, deterministic cleanup, and sanitizer-backed validation.

## Implementation Summary

The lab implements an owned integer buffer with:

- initialization;
- optional initial allocation;
- checked capacity growth;
- `realloc`-based resizing;
- push operations;
- deterministic destruction.

Ownership remains inside the buffer abstraction after successful initialization.

## Runtime Observations

The demonstration inserted ten values and produced:

```text
size=10 capacity=16 values: 10 20 30 40 50 60 70 80 90 100
```

The capacity grew beyond the number of inserted elements according to the implemented growth policy.

After destruction, the tests verified that the internal pointer, size, and capacity were reset.

## Failure Modes Considered

The implementation checks:

- null input structures;
- multiplication overflow before allocation;
- capacity-doubling overflow;
- allocation and reallocation failure.

The original allocation is not lost if `realloc` fails.

## Validation

The lab completed:

- a clean strict-warning build;
- automated tests over 100 insertions;
- AddressSanitizer validation;
- UndefinedBehaviorSanitizer validation.

No warnings, memory errors, or sanitizer failures were reported.

## Result

PASS — dynamic allocation, resizing, ownership, and cleanup behaved correctly under tests and sanitizers.
