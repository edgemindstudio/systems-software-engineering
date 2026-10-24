# Reader/Writer Lock — Engineering Report

## Objective

Protect shared state with `pthread_rwlock_t`, allowing synchronized readers while serializing writes.

## Runtime Observations

```text
final_value=1000
expected_final=1000
rwlock_writer_correct=yes
rwlock_shared_state_verified=yes
```

## Engineering Interpretation

The writer acquired the write lock for each update, while reader threads used the read lock for snapshots.

The final value exactly matched the number of writer updates, showing that write-side serialization preserved shared-state correctness.

## Result

PASS — reader/writer lock coordination and writer correctness were verified.
