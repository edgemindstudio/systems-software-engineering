# Allocation Reuse — Engineering Report

## Objective

Compare repeated allocation/free cycles with reuse of a single allocation while performing equivalent memory-touch work.

## Runtime Observations

```text
iterations=500000
repeated_alloc_seconds=0.002281429
reused_alloc_seconds=0.000106719
sink_equal=yes
allocation_reuse_ratio=21.378
allocation_reuse_measurement_verified=yes
```

## Engineering Interpretation

Both implementations produced the same sink value, establishing equivalent observable work.

The corrected benchmark prevents the compiler from trivially eliminating the allocation/free operations by placing allocation and release behind non-inlined helper functions.

On this environment, repeated allocation/free took approximately 21.4 times as long as reusing one allocation.

This demonstrates measurable allocator overhead under the tested workload while avoiding the misleading near-equal result observed before the benchmark was hardened against optimization.

## Result

PASS — allocation overhead and reusable-buffer comparison were credibly measured.
