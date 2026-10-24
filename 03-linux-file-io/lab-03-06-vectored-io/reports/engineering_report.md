# Vectored I/O — Engineering Report

## Objective

Validate scatter/gather I/O with `writev` and `readv`.

## Runtime Observations

The program reported:

```text
writev_bytes=34
readv_bytes=34
part1=TYPE=event
part2=payload=systems-io
part3=END
```

The resulting file contained:

```text
TYPE=event
payload=systems-io
END
```

`strace` showed one vectored write:

```text
writev(..., 3) = 34
```

followed by:

```text
lseek(..., 0, SEEK_SET) = 0
readv(..., 3) = 34
```

## Engineering Interpretation

Vectored I/O lets multiple non-contiguous user-space buffers participate in a single I/O system call.

The experiment demonstrates gather behavior during `writev` and scatter behavior during `readv`, while preserving the logical boundaries represented by the individual `iovec` entries.

## Result

PASS — 34 bytes were transferred successfully through both vectored write and vectored read operations.
