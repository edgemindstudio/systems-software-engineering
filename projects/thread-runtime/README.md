# Thread Runtime

A reusable bounded worker-thread runtime implemented in C with POSIX threads.

The project goes beyond isolated pthread examples by providing a cohesive task execution API with lifecycle management, queue backpressure, completion tracking, graceful shutdown, and concurrency-safe statistics.

## Features

- configurable worker count
- configurable bounded task queue
- blocking submission when the queue is full
- multiple concurrent producers
- condition-variable worker wakeup
- wait-for-idle semantics
- graceful drain-and-shutdown behavior
- rejection of submissions after shutdown
- task accounting and runtime statistics
- high-volume stress testing
- ASan/UBSan validation
- ThreadSanitizer validation
- throughput/consistency benchmark

## Public API

```c
thread_runtime_t *thread_runtime_create(size_t worker_count,
                                        size_t queue_capacity);

int thread_runtime_submit(thread_runtime_t *runtime,
                          thread_runtime_task_fn function,
                          void *context);

int thread_runtime_wait_idle(thread_runtime_t *runtime);

thread_runtime_stats_t
thread_runtime_get_stats(thread_runtime_t *runtime);

void thread_runtime_shutdown(thread_runtime_t *runtime);
void thread_runtime_destroy(thread_runtime_t *runtime);
```

## Backpressure

The task queue is bounded.

When it is full, submitters wait on a condition variable until a worker removes work from the queue. This prevents an overloaded producer from creating an unbounded amount of queued work.

## Shutdown

Shutdown:

1. stops accepting new tasks;
2. wakes blocked submitters and workers;
3. allows already-queued work to drain;
4. joins all workers.

Submissions made after shutdown return an error and increment the rejected-task statistic.

## Testing

```bash
make test
```

The suite checks:

- normal task execution;
- queue saturation/backpressure state;
- multiple concurrent producers;
- shutdown rejection;
- 10,000-task high-volume execution;
- final accounting consistency.

## Sanitizers

```bash
make sanitize
make tsan
```

The first target uses AddressSanitizer and UndefinedBehaviorSanitizer. The second uses ThreadSanitizer.

## Benchmark

```bash
make benchmark
```

The benchmark submits 100,000 tasks to an eight-worker runtime and verifies that all tasks are completed with no queued or active work remaining.

Timing is environment-specific and is not presented as a universal performance claim.

## Full Validation

```bash
./scripts/validate.sh
```

The validation transcript is stored in:

```text
evidence/validation.txt
```

## Scope

This runtime is intentionally focused. It does not implement:

- task priorities;
- work stealing;
- futures/promises;
- cancellation of individual queued tasks;
- CPU affinity;
- NUMA-aware scheduling;
- dynamic worker resizing.

The project focuses on correct coordination, lifecycle semantics, backpressure, and observable concurrent state.
