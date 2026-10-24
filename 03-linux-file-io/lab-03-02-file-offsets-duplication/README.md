# Lab 03-02 — File Offsets and Descriptor Duplication

## Objective

Demonstrate the relationship among file descriptors, open file descriptions, and shared kernel-managed offsets.

## Key Experiment

- open a file;
- duplicate the descriptor with `dup`;
- read through both descriptors and observe the shared offset;
- open the pathname again to create an independent open file description;
- use `lseek` to inspect and reset offsets.
