# Lab 03-05 — Sparse Files

## Objective

Create a file containing a hole and compare logical file size with physical block allocation.

## Engineering Focus

- `open`
- `lseek`
- sparse extents
- `stat`
- logical bytes versus allocated blocks
- `SEEK_DATA` / `SEEK_HOLE` where supported
