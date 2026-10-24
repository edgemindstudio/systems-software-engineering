# Thread Lifecycle — Engineering Report

## Objective

Create multiple POSIX threads with structured arguments, collect per-thread results, and join every thread deterministically.

## Runtime Observations

```text
thread=0 input=2 result=4
thread=1 input=3 result=9
thread=2 input=4 result=16
thread=3 input=5 result=25
all_threads_joined=yes
thread_lifecycle_verified=yes
```

## Engineering Interpretation

Each worker received its own task structure and independently computed the expected result.

The main thread joined every worker before exiting, proving deterministic lifecycle management and preventing detached or abandoned worker state.

## Result

PASS — thread creation, argument passing, result propagation, and joining were verified.
