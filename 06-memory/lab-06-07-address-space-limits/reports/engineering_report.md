# Address-Space Limits — Engineering Report

## Objective

Apply an `RLIMIT_AS` limit in a child process and verify that an oversized allocation fails cleanly.

## Runtime Observations

```text
allocation_failed_under_limit=yes
child_exit_status=0
controlled_failure_verified=yes
rlimit_allocation_failure_verified=yes
```

## Engineering Interpretation

The child established a restricted virtual-address-space limit before attempting a substantially larger allocation.

`malloc()` returned failure rather than destabilizing the parent process.

The child then exited successfully, showing that allocation failure can be detected and handled as an ordinary error path.

## Result

PASS — controlled address-space exhaustion and graceful allocation failure were verified.
