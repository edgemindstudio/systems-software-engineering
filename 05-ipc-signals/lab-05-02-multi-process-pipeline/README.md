# Lab 05-02 — Multi-Process Pipeline

## Objective

Construct a two-stage pipeline equivalent to:

```text
printf "alpha\nbeta\nalpha\n" | sort | uniq -c
```

The implementation uses `pipe`, `fork`, `dup2`, `exec`, and `waitpid`.
