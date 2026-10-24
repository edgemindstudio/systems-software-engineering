# 03 — Linux File I/O

## Purpose

This section develops practical Linux file-I/O skills at the system-call level.

The labs focus on file descriptors, kernel-managed offsets, filesystem metadata, directory traversal, sparse files, vectored I/O, memory mapping, advisory locking, and robust partial-I/O handling.

## Employer-Facing Outcomes

After completing this section, the repository demonstrates practical ability to:

- use `open`, `read`, `write`, `close`, and `lseek`;
- distinguish file descriptors from open file descriptions;
- handle short reads/writes and `EINTR`;
- inspect metadata with `stat`, `fstat`, and `lstat`;
- reason about hard links, symbolic links, and inode identity;
- traverse directories with `opendir` and `readdir`;
- create and inspect sparse files;
- use `readv` and `writev` for scatter/gather I/O;
- map files with `mmap` and synchronize changes with `msync`;
- coordinate cooperating processes with advisory `fcntl` locks;
- inspect syscall behavior with `strace`;
- preserve reproducible runtime evidence.

## Labs

| Lab | Topic | Core Engineering Evidence |
|---|---|---|
| 03-01 | File Descriptors and Robust I/O | `open/read/write/close`, copy verification, syscall trace |
| 03-02 | File Offsets and Descriptor Duplication | shared offsets, `dup`, independent `open` |
| 03-03 | File Metadata and Links | `stat`, `fstat`, `lstat`, inode/link behavior |
| 03-04 | Directory Traversal | `opendir`, `readdir`, entry classification |
| 03-05 | Sparse Files | logical size vs physical allocation, hole inspection |
| 03-06 | Vectored I/O | `writev`, `readv`, scatter/gather buffers |
| 03-07 | Memory-Mapped File I/O | `mmap`, `MAP_SHARED`, `msync`, persistent mutation |
| 03-08 | Advisory File Locking | `fcntl` locks, contention, release behavior |

## Quality Standard

Every lab contains source code, a strict-warning Makefile, a repeatable validation script, retained runtime evidence, a README, and an engineering report.

Generated build and runtime workspace artifacts are excluded from version control.

## Section Validation

Run:

```bash
./scripts/validate_all.sh
```

The section validator performs clean builds, executes deterministic experiments, captures relevant syscall/filesystem behavior, and writes `evidence/validation.txt` for each lab.

## Completed Results

All eight labs passed on the development environment.

Key observed results included:

- robust descriptor-based file copying with matching SHA-256 hashes;
- `strace` evidence showing `openat`, `read`, `write`, and `close` activity;
- duplicated descriptors sharing a file offset while a separately opened descriptor retained an independent offset;
- hard links sharing an inode and symbolic links being distinguishable through `lstat`;
- deterministic directory-entry classification;
- a sparse file with a logical size of 1,048,580 bytes while only 8,192 bytes were physically allocated;
- one `writev` and one `readv` transferring a 34-byte scatter/gather message;
- a `MAP_SHARED` file mapping whose mutation persisted after `msync`;
- advisory-lock contention where a second process received a busy result while the first process held the lock and succeeded after release.

## Progress

- Lab 03-01 — File Descriptors and Robust I/O: Complete
- Lab 03-02 — File Offsets and Descriptor Duplication: Complete
- Lab 03-03 — File Metadata and Links: Complete
- Lab 03-04 — Directory Traversal: Complete
- Lab 03-05 — Sparse Files: Complete
- Lab 03-06 — Vectored I/O: Complete
- Lab 03-07 — Memory-Mapped File I/O: Complete
- Lab 03-08 — Advisory File Locking: Complete

Domain 03 — Linux File I/O: Complete
