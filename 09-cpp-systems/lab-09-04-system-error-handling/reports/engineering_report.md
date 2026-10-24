# System Error Handling — Engineering Report

## Objective

Wrap POSIX `open()` with `std::system_error` and verify that failure propagates safely without compromising resource cleanup.

## Runtime Observations

```text
good_open_fd_nonnegative=yes
bad_open_caught_system_error=yes
error_value=2
exception_path_survived=yes
system_error_path_verified=yes
```

## Engineering Interpretation

Opening `/etc/hosts` succeeded and produced a valid descriptor managed by the RAII wrapper.

Opening the intentionally invalid path failed and was translated into `std::system_error`.

The captured error value was `2`, corresponding to `ENOENT` on the tested Linux environment.

Program execution continued through the exception path, showing that system-call failures can be surfaced as structured C++ errors while resource lifetimes remain controlled.

## Result

PASS — POSIX-to-C++ error propagation and exception-safe resource handling were verified.
