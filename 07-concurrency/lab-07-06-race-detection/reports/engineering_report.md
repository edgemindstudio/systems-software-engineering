# Race Detection — Engineering Report

## Objective

Demonstrate the effect of unsynchronized shared-state mutation and verify that ThreadSanitizer can diagnose the resulting race.

## Runtime Observations

The normal run reported:

```text
expected=400000
observed=156918
intentional_race_present=yes
```

The ThreadSanitizer run reported:

```text
WARNING: ThreadSanitizer: data race
```

The diagnostic identified concurrent writes in `worker()` to the global `counter`.

The instrumented run also showed another incorrect result:

```text
expected=400000
observed=229557
```

Validation concluded:

```text
tsan_data_race_detected=yes
intentional_race_source_verified=yes
```

## Engineering Interpretation

The unsynchronized `++counter` operation is a read-modify-write sequence and is not atomic.

Concurrent execution caused lost updates, which explains why the observed values were far below the expected total.

ThreadSanitizer independently confirmed that multiple threads were writing the same global location without synchronization.

This provides both behavioral evidence and tooling evidence for the race condition.

## Result

PASS — the intentional data race manifested at runtime and was detected by ThreadSanitizer.
