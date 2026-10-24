# File Descriptors and Robust I/O — Engineering Report

## Objective

Validate descriptor-based file copying using Linux/POSIX system calls and explicit short-I/O handling.

## Implementation Summary

The program uses:

- `open`;
- `read`;
- `write`;
- `close`;
- an explicit `write_all` loop;
- retry behavior for `EINTR`;
- explicit error propagation through return status and `errno`.

## Runtime Observations

The validation copied a controlled text file and produced identical SHA-256 hashes for source and destination:

```text
4fdbc441ea7b546100e086ac1e4fc5ae6749b7314311c99db05be450eca12996
```

for both files.

`strace -c` recorded:

```text
read    3 calls
write   1 call
close   4 calls
openat  4 calls
```

with 12 traced calls total.

## Engineering Interpretation

A successful `read` does not imply that a single `write` will necessarily consume the entire buffer, so the implementation loops until all bytes are written.

The matching hashes verify content integrity independently of the program's own success status.

The syscall trace confirms that the implementation exercised the expected descriptor-level kernel interfaces.

## Result

PASS — robust file copying, content verification, and syscall-level behavior were validated successfully.
