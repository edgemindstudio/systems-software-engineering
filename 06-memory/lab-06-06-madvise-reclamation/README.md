# Lab 06-06 — Memory Reclamation Advice

## Objective

Use `madvise(MADV_DONTNEED)` on a private anonymous mapping and verify that later reads observe zero-filled pages.
