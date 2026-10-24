# Lab 06-03 — Memory Protection and Guard Pages

## Objective

Create a two-page mapping, convert the second page to `PROT_NONE`, then verify in a child process that touching the guard page terminates with `SIGSEGV`.
