# Atomic Counter — Engineering Report

## Objective

Use C11 atomic operations to update a shared counter without a mutex.

## Runtime Observations

```text
expected=800000
actual=800000
atomic_counter_correct=yes
atomic_increment_verified=yes
```

## Engineering Interpretation

Eight threads performed 800,000 total atomic increments.

The exact final value confirms that `atomic_fetch_add_explicit()` prevented lost updates even without a mutex.

The experiment used relaxed memory ordering because only atomicity of the counter operation was required.

## Result

PASS — atomic shared-counter correctness was verified.
