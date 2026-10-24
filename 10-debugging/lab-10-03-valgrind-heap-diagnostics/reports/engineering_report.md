# Valgrind Heap Diagnostics — Engineering Report

## Objective

Introduce a deliberate use-after-free and verify that Valgrind Memcheck identifies the invalid heap access.

## Runtime Observations

```text
Invalid read of size 4
main (src/main.c:14)
Address ... is 0 bytes inside a block of size 4 free'd
free ... main (src/main.c:12)
malloc ... main (src/main.c:6)
post_free_value=123
in use at exit: 0 bytes in 0 blocks
ERROR SUMMARY: 1 errors from 1 contexts
valgrind_error_exit_code=66
valgrind_invalid_read_verified=yes
```

## Engineering Interpretation

Valgrind reconstructed the allocation lifecycle of the invalid address: allocation, free, and subsequent read.

The diagnostic showed that the defect was a use-after-free rather than a leak. The heap summary confirmed that all allocated blocks had been freed by process exit.

The configured nonzero error exit code made the diagnostic machine-checkable by the validation harness.

## Result

PASS — Valgrind use-after-free detection and heap-lifecycle evidence were verified.
