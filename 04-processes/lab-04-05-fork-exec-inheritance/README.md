# Lab 04-05 — Inheritance Across Fork/Exec

## Objective

Inspect process state inherited across `fork()` and retained across `exec()`.

The experiment covers:

- current working directory;
- environment variables;
- an ordinary open file descriptor;
- a descriptor marked `FD_CLOEXEC`.

The exec target probes both descriptor numbers directly.
