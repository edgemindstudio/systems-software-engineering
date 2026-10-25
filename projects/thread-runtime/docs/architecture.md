# Thread Runtime Architecture

## Runtime State

The runtime owns:

- an array of worker `pthread_t` handles;
- a fixed-capacity circular task queue;
- queue head/tail indices;
- queued/active/submitted/completed/rejected counters;
- one mutex protecting shared runtime state;
- condition variables for available work, queue space, and idle-state notification.

## Queue

The queue is a circular buffer of `(function, context)` task records.

Submission waits while the queue is full and the runtime is still accepting work.

Workers remove one task at a time from the queue and signal `queue_space` after making capacity available.

## Worker Lifecycle

Each worker:

1. acquires the runtime mutex;
2. waits while no work is available and shutdown has not begun;
3. exits only when shutdown is active and the queue is empty;
4. removes one task;
5. increments active-task count;
6. releases the mutex;
7. executes the task;
8. reacquires the mutex;
9. decrements active-task count;
10. increments completed-task count;
11. broadcasts the idle condition when queue and active counts reach zero.

Task functions execute without the runtime mutex held.

## Backpressure

`thread_runtime_submit()` blocks when the queue is full.

This is intentional. Queue capacity is a resource bound, and producers must observe that bound rather than creating unbounded pending work.

## Idle Semantics

The runtime is idle only when both conditions hold:

```text
queued_tasks == 0
active_tasks == 0
```

`thread_runtime_wait_idle()` waits on a dedicated condition variable until that predicate becomes true.

## Shutdown

Shutdown changes two state flags under the mutex:

```text
accepting_tasks = 0
stopping = 1
```

It then wakes workers and blocked producers.

Workers continue processing already-queued tasks. Once the queue is empty, they exit and are joined.

## Statistics

All statistics are read while holding the runtime mutex, producing a coherent snapshot of runtime-controlled fields.

## Concurrency Validation

The project deliberately combines:

- normal integration/stress tests;
- AddressSanitizer and UndefinedBehaviorSanitizer;
- ThreadSanitizer.

The concurrent-producer test exercises multiple submitter threads concurrently with the worker pool, while the high-volume test verifies final accounting after 10,000 tasks.
