# Syscall Amortization — Engineering Report

## Objective

Write the same total number of bytes using many 1-byte `write()` calls and fewer 4 KiB `write()` calls.

## Runtime Observations

```text
total_bytes=33554432
one_byte_seconds=2.704782264
four_kib_seconds=0.000635290
small_to_large_ratio=4257.555
same_total_bytes=yes
syscall_amortization_measurement_verified=yes
```

## Engineering Interpretation

Both cases wrote the same 32 MiB total to `/dev/null`, making I/O granularity the principal controlled difference.

The 1-byte case required vastly more system calls than the 4 KiB case.

On this environment, the small-write workload took approximately 4257.6 times as long, illustrating the cost of crossing the user/kernel boundary repeatedly and the benefit of amortizing syscall overhead across larger transfers.

The absolute ratio is environment-specific.

## Result

PASS — syscall-amortization behavior was measured with equivalent total byte counts.
