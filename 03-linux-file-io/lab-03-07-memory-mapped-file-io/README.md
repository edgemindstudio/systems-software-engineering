# Lab 03-07 — Memory-Mapped File I/O

## Objective

Map a regular file into memory, modify bytes through the mapping, synchronize the mapping, and verify persistence through normal file I/O.

## Engineering Focus

- `open`
- `fstat`
- `mmap`
- `MAP_SHARED`
- `msync`
- `munmap`
- mapping lifetime
