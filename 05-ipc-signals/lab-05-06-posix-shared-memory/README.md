# Lab 05-06 — POSIX Shared Memory

## Objective

Create a POSIX shared-memory object, size it with `ftruncate`, map it with `mmap`, and demonstrate parent/child visibility of shared state.

A pipe is used only as a deterministic readiness notification so the shared-memory observation is race-free.
