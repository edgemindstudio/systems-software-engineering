# Memory Reclamation Advice — Engineering Report

## Objective

Apply `madvise(MADV_DONTNEED)` to a private anonymous mapping and verify the resulting observable contents.

## Runtime Observations

```text
before=64,65,66,67
after=0,0,0,0
madv_dontneed_zero_fill_verified=yes
```

## Engineering Interpretation

The program wrote distinct values into four pages, then advised the kernel that the pages were no longer needed.

Subsequent reads observed zero-filled pages.

For this private anonymous mapping on the tested Linux environment, `MADV_DONTNEED` discarded the previous anonymous-page contents so later access reconstructed zero-filled memory.

## Result

PASS — anonymous-page discard and zero-fill behavior were verified.
