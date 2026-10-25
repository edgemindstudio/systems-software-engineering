#ifndef THREAD_RUNTIME_THREAD_RUNTIME_H
#define THREAD_RUNTIME_THREAD_RUNTIME_H

#include <stddef.h>

typedef struct thread_runtime thread_runtime_t;

typedef void (*thread_runtime_task_fn)(void *context);

typedef struct {
    size_t worker_count;
    size_t queue_capacity;
    size_t queued_tasks;
    size_t active_tasks;
    size_t submitted_tasks;
    size_t completed_tasks;
    size_t rejected_tasks;
    int accepting_tasks;
} thread_runtime_stats_t;

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

#endif
