# Lab 03-06 — Vectored I/O

## Objective

Use scatter/gather I/O to transfer multiple buffers in a single system call.

## Engineering Focus

- `struct iovec`
- `writev`
- `readv`
- scatter/gather semantics
- multi-buffer message layout
