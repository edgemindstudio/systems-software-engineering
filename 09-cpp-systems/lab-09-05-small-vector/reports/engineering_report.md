# Small Vector Container — Engineering Report

## Objective

Implement a minimal contiguous integer container with explicit size/capacity invariants, geometric growth, and checked element access.

## Runtime Observations

```text
size=10
capacity=16
first=10
last=100
bounds_exception_caught=yes
container_invariants_verified=yes
small_vector_verified=yes
```

## Engineering Interpretation

The container stored 10 values while growing geometrically from its initial allocation to a final capacity of 16.

The first and last stored values remained correct after reallocations, demonstrating successful element preservation across growth.

Accessing index 10 for a 10-element container raised the expected `std::out_of_range` exception.

## Result

PASS — container growth, data preservation, size/capacity invariants, and bounds checking were verified.
