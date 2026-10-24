# Lab 06-04 — Fork Copy-on-Write Semantics

## Objective

Map a private anonymous page before `fork()`, mutate the same virtual location independently in parent and child, and verify isolation.
