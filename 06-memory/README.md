# 06 — Memory

## Purpose

This section develops practical Linux process-memory and virtual-memory skills beyond basic dynamic allocation.

The labs focus on address-space layout, anonymous mappings, page protection, copy-on-write semantics, page residency, page faults, reclamation advice, and address-space limits.

## Employer-Facing Outcomes

After completing this section, the repository demonstrates practical ability to:

- inspect representative code, global, heap, and stack addresses;
- correlate runtime addresses with `/proc` memory mappings;
- query and reason about system page size;
- create and release anonymous mappings with `mmap` and `munmap`;
- protect pages with `mprotect`;
- construct and validate guard pages;
- reason about private mappings and copy-on-write after `fork`;
- inspect page residency with `mincore`;
- observe minor page faults with `getrusage`;
- use `madvise(MADV_DONTNEED)` on anonymous memory;
- enforce address-space limits with `RLIMIT_AS`;
- isolate expected faults and allocation failures in child processes;
- preserve reproducible runtime evidence.

## Labs

| Lab | Topic | Core Engineering Evidence |
|---|---|---|
| 06-01 | Virtual Address Space | code/global/heap/stack addresses, `/proc` mappings |
| 06-02 | Anonymous Memory Mapping | 4096-byte pages, anonymous `mmap`, read/write, `munmap` |
| 06-03 | Memory Protection and Guard Pages | `mprotect`, `PROT_NONE`, expected `SIGSEGV` |
| 06-04 | Fork Copy-on-Write Semantics | parent/child memory isolation after mutation |
| 06-05 | Page Residency and Faults | `mincore`, 64 touched pages, minor faults |
| 06-06 | Memory Reclamation Advice | `MADV_DONTNEED`, anonymous-page reset |
| 06-07 | Address-Space Limits | `RLIMIT_AS`, controlled allocation failure |

## Completed Results

All seven labs passed on the development environment.

Key observed results included:

- distinct runtime addresses for code, global, heap, and stack objects;
- a 4096-byte system page size;
- successful 8192-byte anonymous mapping and `munmap`;
- a guard-page access terminating the child with signal 11 (`SIGSEGV`);
- child mutation to `200` leaving the parent's private page at `100`;
- parent subsequently mutating its own page to `300`;
- `resident_before=0` and `resident_after=64` after touching 64 pages;
- `minor_fault_delta=64`;
- `MADV_DONTNEED` resetting page contents from `64,65,66,67` to `0,0,0,0`;
- controlled allocation failure under `RLIMIT_AS`;
- successful child exit after the expected allocation failure.

## Progress

- Lab 06-01 — Virtual Address Space: Complete
- Lab 06-02 — Anonymous Memory Mapping: Complete
- Lab 06-03 — Memory Protection and Guard Pages: Complete
- Lab 06-04 — Fork Copy-on-Write Semantics: Complete
- Lab 06-05 — Page Residency and Faults: Complete
- Lab 06-06 — Memory Reclamation Advice: Complete
- Lab 06-07 — Address-Space Limits: Complete

Domain 06 — Memory: Complete
