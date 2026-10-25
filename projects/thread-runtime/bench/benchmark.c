#include "thread_runtime/thread_runtime.h"

#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TASK_COUNT 100000U

typedef struct {
    atomic_ulong *counter;
} task_ctx_t;

static void increment_task(void *context)
{
    task_ctx_t *task = context;
    (void)atomic_fetch_add_explicit(task->counter,
                                    1U,
                                    memory_order_relaxed);
}

static double seconds_between(struct timespec start,
                              struct timespec end)
{
    return (double)(end.tv_sec - start.tv_sec) +
           (double)(end.tv_nsec - start.tv_nsec) / 1000000000.0;
}

int main(void)
{
    thread_runtime_t *runtime =
        thread_runtime_create(8U, 256U);

    if (runtime == NULL) {
        return 1;
    }

    task_ctx_t *tasks = calloc(TASK_COUNT, sizeof *tasks);
    if (tasks == NULL) {
        thread_runtime_destroy(runtime);
        return 1;
    }

    atomic_ulong counter = 0U;

    for (size_t i = 0U; i < TASK_COUNT; ++i) {
        tasks[i].counter = &counter;
    }

    struct timespec start;
    struct timespec end;

    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
        free(tasks);
        thread_runtime_destroy(runtime);
        return 1;
    }

    for (size_t i = 0U; i < TASK_COUNT; ++i) {
        if (thread_runtime_submit(runtime,
                                  increment_task,
                                  &tasks[i]) != 0) {
            free(tasks);
            thread_runtime_destroy(runtime);
            return 1;
        }
    }

    if (thread_runtime_wait_idle(runtime) != 0) {
        free(tasks);
        thread_runtime_destroy(runtime);
        return 1;
    }

    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0) {
        free(tasks);
        thread_runtime_destroy(runtime);
        return 1;
    }

    thread_runtime_stats_t stats =
        thread_runtime_get_stats(runtime);

    printf("tasks=%u\n", (unsigned int)TASK_COUNT);
    printf("workers=%zu\n", stats.worker_count);
    printf("queue_capacity=%zu\n", stats.queue_capacity);
    printf("elapsed_seconds=%.9f\n",
           seconds_between(start, end));
    printf("submitted_tasks=%zu\n", stats.submitted_tasks);
    printf("completed_tasks=%zu\n", stats.completed_tasks);
    printf("queued_tasks=%zu\n", stats.queued_tasks);
    printf("active_tasks=%zu\n", stats.active_tasks);
    printf("counter=%lu\n",
           atomic_load_explicit(&counter,
                                memory_order_relaxed));

    const int ok =
        stats.submitted_tasks == TASK_COUNT &&
        stats.completed_tasks == TASK_COUNT &&
        stats.queued_tasks == 0U &&
        stats.active_tasks == 0U &&
        atomic_load_explicit(&counter,
                             memory_order_relaxed) == TASK_COUNT;

    printf("runtime_consistent=%s\n", ok ? "yes" : "no");

    free(tasks);
    thread_runtime_destroy(runtime);
    return ok ? 0 : 1;
}
