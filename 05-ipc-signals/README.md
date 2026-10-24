# 05 — IPC & Signals

## Purpose

This section develops practical Linux inter-process communication and signal-handling skills.

The labs focus on anonymous pipes, process pipelines, safe signal handling, masked/pending signals, named FIFOs, and POSIX shared memory.

## Employer-Facing Outcomes

After completing this section, the repository demonstrates practical ability to:

- create and use anonymous pipes;
- reason about pipe EOF and descriptor closure;
- construct multi-process pipelines with `pipe`, `fork`, `dup2`, and `exec`;
- install handlers with `sigaction`;
- keep signal-handler work async-signal-safe;
- use `kill` for process notification;
- block and restore signals with `sigprocmask`;
- detect pending signals with `sigpending`;
- avoid signal wait races with `sigsuspend`;
- create filesystem-visible IPC endpoints with `mkfifo`;
- use POSIX shared memory with `shm_open`, `ftruncate`, and `mmap`;
- coordinate shared-memory visibility deterministically;
- clean up FIFO and shared-memory resources;
- inspect IPC/process behavior with `strace`;
- preserve reproducible runtime evidence.

## Labs

| Lab | Topic | Core Engineering Evidence |
|---|---|---|
| 05-01 | Anonymous Pipes | parent/child byte stream, EOF behavior, descriptor hygiene |
| 05-02 | Multi-Process Pipeline | `pipe`, `fork`, `dup2`, `exec`, staged stdout→stdin flow |
| 05-03 | Safe Signal Handling | `sigaction`, `kill`, `sig_atomic_t`, async-signal-safe handler |
| 05-04 | Signal Masks and `sigsuspend` | blocked delivery, pending signal, race-free wakeup |
| 05-05 | Named FIFO IPC | `mkfifo`, reader/writer processes, cleanup |
| 05-06 | POSIX Shared Memory | `shm_open`, `ftruncate`, `mmap`, visibility, unlink cleanup |

## Completed Results

All six labs passed on the development environment.

Key observed results included:

- successful anonymous-pipe delivery of `hello-through-pipe`;
- EOF detection after the final pipe writer closed;
- a real two-stage `sort | uniq -c` pipeline producing `2 alpha` and `1 beta`;
- `strace` evidence showing `pipe2`, `clone`, `dup2`, `execve`, `close`, and `wait4`;
- successful `SIGUSR1` delivery through a `sigaction` handler;
- blocked `SIGUSR1` becoming pending before `sigsuspend`;
- race-free handler wakeup after temporarily restoring the prior mask;
- successful named-FIFO delivery and FIFO cleanup;
- shared-memory visibility of a readiness flag, integer value, and message;
- successful `shm_unlink` cleanup after the shared-memory experiment.

## Progress

- Lab 05-01 — Anonymous Pipes: Complete
- Lab 05-02 — Multi-Process Pipeline: Complete
- Lab 05-03 — Safe Signal Handling: Complete
- Lab 05-04 — Signal Masks and `sigsuspend`: Complete
- Lab 05-05 — Named FIFO IPC: Complete
- Lab 05-06 — POSIX Shared Memory: Complete

Domain 05 — IPC & Signals: Complete
