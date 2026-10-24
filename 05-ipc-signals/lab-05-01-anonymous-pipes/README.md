# Lab 05-01 — Anonymous Pipes

## Objective

Create a unidirectional parent-to-child byte stream using `pipe()` and `fork()`.

The lab emphasizes correct closure of unused pipe ends and EOF delivery when the last writer closes.
