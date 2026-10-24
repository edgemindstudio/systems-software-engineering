# GDB Segmentation-Fault Analysis — Engineering Report

## Objective

Trigger a deterministic null-pointer dereference and use GDB to capture the crash signal, source line, and call stack.

## Runtime Observations

```text
about_to_crash=yes
Program received signal SIGSEGV, Segmentation fault.
crash_here (ptr=0x0) at src/main.c:5
#0 crash_here (ptr=0x0) at src/main.c:5
#1 main () at src/main.c:13
gdb_return_code=0
gdb_segfault_backtrace_verified=yes
```

## Engineering Interpretation

GDB stopped exactly at the null dereference in `crash_here()`.

The backtrace connected the faulting function to its caller in `main()`, demonstrating how debugger stack evidence narrows a crash from a process-level symptom to a specific source statement and call path.

The GDB batch invocation completed successfully after collecting the diagnostic.

## Result

PASS — GDB crash localization and backtrace analysis were verified.
