# Mutex-Protected Counter — Engineering Report

## Objective

Protect repeated shared-counter increments with `pthread_mutex_t`.

## Runtime Observations

```text
expected=400000
actual=400000
mutex_counter_correct=yes
mutex_serialization_verified=yes
```

## Engineering Interpretation

Eight threads performed a total of 400,000 increments on shared state.

Because every increment occurred inside the mutex-protected critical section, the final value exactly matched the expected result.

This demonstrates serialized access to shared mutable state and prevention of lost updates.

## Result

PASS — mutex-protected shared-state mutation was verified.
