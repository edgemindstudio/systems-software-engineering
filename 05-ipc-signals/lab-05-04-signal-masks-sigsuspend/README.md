# Lab 05-04 — Signal Masks and `sigsuspend`

## Objective

Demonstrate blocked signal delivery, pending signals, and race-free waiting with `sigsuspend()`.

The parent blocks `SIGUSR1` before forking, verifies that the signal becomes pending, then atomically swaps masks and sleeps with `sigsuspend`.
