# AddressSanitizer — Engineering Report

## Objective

Trigger a heap-buffer overflow and verify that AddressSanitizer identifies the invalid access and allocation boundary.

## Runtime Observations

```text
ERROR: AddressSanitizer: heap-buffer-overflow
WRITE of size 4
main ... src/main.c:14:15
address ... is located 0 bytes after 16-byte region
allocated ... src/main.c:5:19
SUMMARY: AddressSanitizer: heap-buffer-overflow
asan_return_code=1
asan_heap_overflow_verified=yes
```

## Engineering Interpretation

The program allocated space for four integers, producing a 16-byte heap region, then wrote a fifth integer immediately beyond the allocation.

AddressSanitizer identified both the invalid write and the exact allocation boundary, providing more precise spatial-memory diagnostics than a generic crash alone would provide.

## Result

PASS — AddressSanitizer heap-buffer-overflow detection was verified.
