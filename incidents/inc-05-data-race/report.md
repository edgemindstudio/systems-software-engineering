# INC-05 — Data Race

## Symptom

Two threads update the same global counter without synchronization.

## Reproduction

Two worker threads repeatedly execute `++counter` concurrently.

## Evidence

ThreadSanitizer reported:

```text
WARNING: ThreadSanitizer: data race
```

Both conflicting accesses were writes in `worker()` at `src/main.c:10`, and the report identified the shared location as global `counter`.

```text
tsan_exit_code=66
```

## Root Cause

`++counter` is a read-modify-write operation. Without synchronization, the two threads can interleave accesses to the same memory location, producing a data race and undefined behavior.

## Remediation

Protect the shared state with a mutex or replace the counter with an appropriate atomic operation when the required semantics permit it.

## Verification

ThreadSanitizer directly reported the conflicting thread accesses and their creation sites.

## Engineering Lesson

A numerically plausible result does not prove concurrent correctness. Race detectors expose invalid access patterns that ordinary output-based testing can miss.
