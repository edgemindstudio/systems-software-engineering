# Lab 03-08 — Advisory File Locking

## Objective

Demonstrate cooperating-process coordination using POSIX advisory record locks.

## Engineering Focus

- `fcntl`
- `F_SETLK`
- write locks
- lock contention
- process ownership of advisory locks
- lock release on close/process exit

## Validation

The validation script starts one process that holds a write lock, verifies that a second process cannot acquire it, waits for release, then verifies acquisition succeeds.
