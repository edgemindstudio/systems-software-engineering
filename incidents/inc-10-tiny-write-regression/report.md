# INC-10 — Tiny-Write Performance Regression

## Symptom

Writing the same byte volume through extremely small write operations causes a severe performance regression.

## Reproduction

The test writes 8 MiB to `/dev/null` using 1-byte writes and then repeats the same total transfer with 4 KiB writes.

## Evidence

```text
total_bytes=8388608
tiny_write_seconds=0.671524047
batched_write_seconds=0.000156884
tiny_to_batched_ratio=4280.386
regression_reproduced=yes
```

## Root Cause

The tiny-write implementation performs vastly more system calls. The per-call user/kernel transition and syscall handling overhead dominates the otherwise trivial transfer.

## Remediation

Batch data into larger writes when latency and protocol semantics allow it.

## Verification

Both cases transferred the same total byte count, while the observed tiny/batched timing ratio on this environment was approximately 4280.4.

## Engineering Lesson

Performance failures often come from operation granularity rather than algorithmic complexity alone. System-call frequency is a first-class performance variable.

The measured ratio is environment-specific and should not be treated as a universal constant.
