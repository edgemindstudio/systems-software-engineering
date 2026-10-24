# RAII File Descriptor — Engineering Report

## Objective

Wrap a POSIX file descriptor in a move-only C++ type and verify deterministic cleanup when the owning object leaves scope.

## Runtime Observations

```text
fd_valid_inside_scope=yes
raw_fd_closed_after_scope=yes
raii_fd_cleanup_verified=yes
```

## Engineering Interpretation

The file descriptor remained valid while the RAII wrapper owned it.

After the wrapper left scope, a direct `fcntl()` check on the original raw descriptor failed with `EBADF`, proving that the destructor closed the resource.

This demonstrates deterministic resource cleanup tied to C++ object lifetime.

## Result

PASS — RAII-based file-descriptor ownership and cleanup were verified.
