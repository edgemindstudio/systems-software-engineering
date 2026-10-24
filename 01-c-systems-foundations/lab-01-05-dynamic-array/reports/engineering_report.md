# Dynamic Array — Engineering Report

## Objective

Build a reusable vector-style container that maintains explicit size/capacity invariants while supporting growth, indexed access, insertion, and removal.

## Implementation Summary

The dynamic array provides:

- initialization and destruction;
- geometric capacity growth;
- push-back;
- bounds-checked indexed access;
- insertion using `memmove`;
- removal using `memmove`.

The core invariant is:

```text
size <= capacity
```

with initialized elements occupying the range `[0, size)`.

## Runtime Observations

The demonstration produced:

```text
size=5 capacity=8 removed=40 values: 10 20 25 30 50
```

This confirmed successful growth, insertion into the middle of the array, removal, element shifting, and preservation of the remaining sequence.

The tests also exercised a larger set of insertions and invalid index handling.

## Failure Modes Considered

The implementation rejects invalid indices and checks for capacity-growth overflow and allocation failure.

Existing storage remains valid if reallocation fails.

## Validation

The lab completed:

- a clean strict-warning build;
- automated tests;
- AddressSanitizer validation;
- UndefinedBehaviorSanitizer validation.

No warnings or sanitizer errors were reported.

## Result

PASS — the vector abstraction maintained its invariants through growth, insertion, access, and removal.
