# Lab 05-03 — Safe Signal Handling

## Objective

Install a signal handler with `sigaction()`, send `SIGUSR1` with `kill()`, and keep handler work minimal and async-signal-safe.

The handler only updates `volatile sig_atomic_t` state and writes a fixed message with `write()`.
