# Sparse Files — Engineering Report

## Objective

Create and inspect a sparse file containing a large logical hole.

## Runtime Observations

The program created a file with:

```text
logical_size=1048580
allocated_512_blocks=16
allocated_bytes=8192
```

Filesystem inspection independently reported:

```text
size=1048580
blocks=16
block_size=512
```

`SEEK_DATA` / `SEEK_HOLE` observations were:

```text
first_data=0
first_hole=4096
```

The validation confirmed:

```text
sparse_allocation_verified=yes
```

## Engineering Interpretation

The file's logical length exceeded 1 MiB, yet only 8 KiB of physical blocks were allocated.

This demonstrates that seeking beyond the current end of file and later writing data can create a hole whose unwritten range reads logically as zeroes without consuming equivalent physical storage.

The observed hole boundary reflects filesystem allocation granularity rather than the exact byte where the earlier write ended.

## Result

PASS — sparse-file creation and the distinction between logical size and physical allocation were demonstrated successfully.
