# Optimization Effects — Engineering Report

## Objective

Compare `-O0` and `-O2` builds of the same computational workload while verifying identical program results.

## Runtime Observations

```text
iterations=80000000

O0:
result=3851012319523133344
elapsed_seconds=0.039192705

O2:
result=3851012319523133344
elapsed_seconds=0.025058530

results_equal=yes
o0_to_o2_ratio=1.564
optimization_comparison_verified=yes
```

## Engineering Interpretation

The unoptimized and optimized binaries produced the exact same computational result, so the timing comparison preserved functional equivalence.

On this development environment, the `-O0` build took approximately 1.56 times as long as the `-O2` build.

The result demonstrates that compiler optimization level can materially change runtime performance even when source-level behavior and final output remain unchanged.

The observed ratio is specific to the workload, compiler, and host environment.

## Result

PASS — optimization-level performance effects were measured with result equivalence verified.
