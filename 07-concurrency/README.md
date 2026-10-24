# 07 — Concurrency

## Purpose

This section develops practical POSIX-thread concurrency skills for Linux systems software.

The labs focus on thread lifecycle, mutual exclusion, condition variables, bounded producer/consumer coordination, atomics, race detection, reader/writer locking, and bounded worker-pool design.

## Employer-Facing Outcomes

After completing this section, the repository demonstrates practical ability to:

- create and join POSIX threads;
- pass structured arguments to worker threads;
- protect shared state with `pthread_mutex_t`;
- coordinate threads with `pthread_cond_t`;
- build bounded producer/consumer queues;
- use C11 atomic operations for shared counters;
- expose and diagnose data races with ThreadSanitizer;
- use `pthread_rwlock_t` for read-heavy shared state;
- build fixed-size worker pools with bounded queues;
- coordinate graceful worker shutdown;
- reason about synchronization predicates and invariants;
- preserve reproducible concurrency evidence.

## Labs

| Lab | Topic | Core Engineering Evidence |
|---|---|---|
| 07-01 | Thread Lifecycle | `pthread_create`, arguments, `pthread_join` |
| 07-02 | Mutex-Protected Counter | exact shared-state mutation under a mutex |
| 07-03 | Condition Variable Coordination | predicate loop and signal/wakeup |
| 07-04 | Bounded Producer/Consumer Queue | mutex plus `not_empty` / `not_full` conditions |
| 07-05 | Atomic Counter | C11 `_Atomic`, `atomic_fetch_add` |
| 07-06 | Race Detection | lost updates plus ThreadSanitizer diagnostic |
| 07-07 | Reader/Writer Lock | serialized writer with reader synchronization |
| 07-08 | Bounded Worker Pool | task queueing, completion accounting, clean shutdown |

## Completed Results

All eight labs passed on the development environment.

Key observed results included:

- four worker threads returning the expected squared results and joining cleanly;
- a mutex-protected counter reaching exactly `400000`;
- condition-variable predicate coordination completing correctly;
- a bounded producer/consumer queue producing the exact sum `5050`;
- an atomic counter reaching exactly `800000`;
- an intentionally racy counter producing only `156918` instead of `400000`;
- ThreadSanitizer detecting a real data race in the intentionally unsafe counter;
- the TSan run identifying concurrent writes to the global `counter`;
- a reader/writer protected shared value reaching exactly `1000`;
- a bounded worker pool completing all 40 tasks;
- worker-pool result sum matching the expected `22140`;
- clean worker shutdown after the queue drained.

## Progress

- Lab 07-01 — Thread Lifecycle: Complete
- Lab 07-02 — Mutex-Protected Counter: Complete
- Lab 07-03 — Condition Variable Coordination: Complete
- Lab 07-04 — Bounded Producer/Consumer Queue: Complete
- Lab 07-05 — Atomic Counter: Complete
- Lab 07-06 — Race Detection: Complete
- Lab 07-07 — Reader/Writer Lock: Complete
- Lab 07-08 — Bounded Worker Pool: Complete

Domain 07 — Concurrency: Complete
