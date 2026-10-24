# File Offsets and Descriptor Duplication — Engineering Report

## Objective

Demonstrate the relationship among file descriptors, open file descriptions, and kernel-managed file offsets.

## Runtime Observations

The controlled file contained:

```text
ABCDE
```

The program produced:

```text
fd1_byte=A
dup_fd2_byte=B
independent_fd3_byte=A
fd1_offset=2
fd2_offset=2
fd3_offset=1
```

## Engineering Interpretation

`fd1` and the descriptor returned by `dup(fd1)` refer to the same open file description and therefore share the same file offset.

Reading one byte through `fd1` advanced the shared offset to 1. Reading through the duplicated descriptor then returned `B` and advanced that same offset to 2.

A separate `open()` created a different open file description, so its first read independently returned `A` and left its offset at 1.

This distinction is fundamental to descriptor duplication, process inheritance, redirection, and concurrent I/O behavior.

## Result

PASS — shared and independent file-offset behavior matched Linux/POSIX semantics.
