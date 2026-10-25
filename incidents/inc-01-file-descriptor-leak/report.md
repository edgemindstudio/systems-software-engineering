# INC-01 — File Descriptor Leak

## Symptom

A process accumulates open file descriptors when resources are opened but not closed.

## Reproduction

The test opens eight additional descriptors for `/dev/null` and measures the process descriptor count before and after opening them.

## Evidence

```text
fds_before=3
fds_after_open=11
fds_after_close=3
leak_reproduced=yes
cleanup_verified=yes
```

## Root Cause

File descriptors remain owned by the process until they are explicitly closed or the process exits. Opening resources without a matching `close()` increases the process descriptor table usage.

## Remediation

Close each descriptor on every successful ownership path and on error paths that occur after acquisition.

## Verification

After closing all eight descriptors, the count returned from 11 to the original value of 3.

## Engineering Lesson

Resource ownership must have a clear release path. Descriptor leaks may remain invisible during short runs but become availability failures in long-lived processes.
