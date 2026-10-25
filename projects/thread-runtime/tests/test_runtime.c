#include "thread_runtime/thread_runtime.h"

#include <assert.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    atomic_ulong *counter;
    unsigned long value;
} add_task_t;

static void add_task(void *context)
{
    add_task_t *task = context;
    (void)atomic_fetch_add_explicit(task->counter,
                                    task->value,
                                    memory_order_relaxed);
}

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int release;
    atomic_int entered;
} blocking_gate_t;

static void blocking_task(void *context)
{
    blocking_gate_t *gate = context;

    (void)atomic_fetch_add_explicit(&gate->entered,
                                    1,
                                    memory_order_relaxed);

    (void)pthread_mutex_lock(&gate->mutex);
    while (gate->release == 0) {
        (void)pthread_cond_wait(&gate->cond, &gate->mutex);
    }
    (void)pthread_mutex_unlock(&gate->mutex);
}

typedef struct {
    thread_runtime_t *runtime;
    add_task_t *tasks;
    size_t begin;
    size_t end;
    atomic_int *errors;
} producer_ctx_t;

static void *producer_main(void *context)
{
    producer_ctx_t *producer = context;

    for (size_t i = producer->begin; i < producer->end; ++i) {
        if (thread_runtime_submit(producer->runtime,
                                  add_task,
                                  &producer->tasks[i]) != 0) {
            (void)atomic_fetch_add_explicit(producer->errors,
                                            1,
                                            memory_order_relaxed);
        }
    }

    return NULL;
}

static void test_basic_execution(void)
{
    thread_runtime_t *runtime = thread_runtime_create(4U, 16U);
    assert(runtime != NULL);

    atomic_ulong counter = 0U;
    add_task_t tasks[100];

    for (size_t i = 0U; i < 100U; ++i) {
        tasks[i].counter = &counter;
        tasks[i].value = 1U;
        assert(thread_runtime_submit(runtime,
                                     add_task,
                                     &tasks[i]) == 0);
    }

    assert(thread_runtime_wait_idle(runtime) == 0);
    assert(atomic_load_explicit(&counter,
                                memory_order_relaxed) == 100U);

    thread_runtime_stats_t stats =
        thread_runtime_get_stats(runtime);

    assert(stats.submitted_tasks == 100U);
    assert(stats.completed_tasks == 100U);
    assert(stats.queued_tasks == 0U);
    assert(stats.active_tasks == 0U);

    thread_runtime_destroy(runtime);
    puts("basic_execution=PASS");
}

static void test_bounded_backpressure(void)
{
    thread_runtime_t *runtime = thread_runtime_create(1U, 1U);
    assert(runtime != NULL);

    blocking_gate_t gate;
    assert(pthread_mutex_init(&gate.mutex, NULL) == 0);
    assert(pthread_cond_init(&gate.cond, NULL) == 0);
    gate.release = 0;
    atomic_init(&gate.entered, 0);

    assert(thread_runtime_submit(runtime,
                                 blocking_task,
                                 &gate) == 0);

    while (atomic_load_explicit(&gate.entered,
                                memory_order_relaxed) == 0) {
        struct timespec pause = {0, 1000000L};
        (void)nanosleep(&pause, NULL);
    }

    assert(thread_runtime_submit(runtime,
                                 blocking_task,
                                 &gate) == 0);

    thread_runtime_stats_t stats =
        thread_runtime_get_stats(runtime);

    assert(stats.worker_count == 1U);
    assert(stats.queue_capacity == 1U);
    assert(stats.active_tasks == 1U);
    assert(stats.queued_tasks == 1U);

    (void)pthread_mutex_lock(&gate.mutex);
    gate.release = 1;
    (void)pthread_cond_broadcast(&gate.cond);
    (void)pthread_mutex_unlock(&gate.mutex);

    assert(thread_runtime_wait_idle(runtime) == 0);

    stats = thread_runtime_get_stats(runtime);
    assert(stats.completed_tasks == 2U);

    thread_runtime_destroy(runtime);
    (void)pthread_cond_destroy(&gate.cond);
    (void)pthread_mutex_destroy(&gate.mutex);

    puts("bounded_backpressure=PASS");
}

static void test_concurrent_producers(void)
{
    enum {
        PRODUCERS = 4,
        TASKS_PER_PRODUCER = 500,
        TOTAL_TASKS = PRODUCERS * TASKS_PER_PRODUCER
    };

    thread_runtime_t *runtime = thread_runtime_create(6U, 64U);
    assert(runtime != NULL);

    atomic_ulong counter = 0U;
    atomic_int errors = 0;

    add_task_t *tasks = calloc(TOTAL_TASKS, sizeof *tasks);
    assert(tasks != NULL);

    for (size_t i = 0U; i < TOTAL_TASKS; ++i) {
        tasks[i].counter = &counter;
        tasks[i].value = 1U;
    }

    pthread_t producers[PRODUCERS];
    producer_ctx_t contexts[PRODUCERS];

    for (size_t i = 0U; i < PRODUCERS; ++i) {
        contexts[i].runtime = runtime;
        contexts[i].tasks = tasks;
        contexts[i].begin = i * TASKS_PER_PRODUCER;
        contexts[i].end = contexts[i].begin + TASKS_PER_PRODUCER;
        contexts[i].errors = &errors;

        assert(pthread_create(&producers[i],
                              NULL,
                              producer_main,
                              &contexts[i]) == 0);
    }

    for (size_t i = 0U; i < PRODUCERS; ++i) {
        assert(pthread_join(producers[i], NULL) == 0);
    }

    assert(thread_runtime_wait_idle(runtime) == 0);

    assert(atomic_load_explicit(&errors,
                                memory_order_relaxed) == 0);
    assert(atomic_load_explicit(&counter,
                                memory_order_relaxed) ==
           (unsigned long)TOTAL_TASKS);

    thread_runtime_stats_t stats =
        thread_runtime_get_stats(runtime);

    assert(stats.submitted_tasks == (size_t)TOTAL_TASKS);
    assert(stats.completed_tasks == (size_t)TOTAL_TASKS);

    free(tasks);
    thread_runtime_destroy(runtime);

    puts("concurrent_producers=PASS");
}

static void test_shutdown_rejects_new_work(void)
{
    thread_runtime_t *runtime = thread_runtime_create(2U, 8U);
    assert(runtime != NULL);

    atomic_ulong counter = 0U;
    add_task_t task = {&counter, 1U};

    assert(thread_runtime_submit(runtime,
                                 add_task,
                                 &task) == 0);

    assert(thread_runtime_wait_idle(runtime) == 0);

    thread_runtime_shutdown(runtime);

    int rc = thread_runtime_submit(runtime,
                                   add_task,
                                   &task);
    assert(rc != 0);

    thread_runtime_stats_t stats =
        thread_runtime_get_stats(runtime);

    assert(stats.accepting_tasks == 0);
    assert(stats.rejected_tasks == 1U);
    assert(stats.completed_tasks == 1U);

    thread_runtime_destroy(runtime);
    puts("shutdown_rejection=PASS");
}

static void test_high_volume(void)
{
    enum { TASK_COUNT = 10000 };

    thread_runtime_t *runtime = thread_runtime_create(8U, 128U);
    assert(runtime != NULL);

    atomic_ulong counter = 0U;
    add_task_t *tasks = calloc(TASK_COUNT, sizeof *tasks);
    assert(tasks != NULL);

    for (size_t i = 0U; i < TASK_COUNT; ++i) {
        tasks[i].counter = &counter;
        tasks[i].value = 1U;
        assert(thread_runtime_submit(runtime,
                                     add_task,
                                     &tasks[i]) == 0);
    }

    assert(thread_runtime_wait_idle(runtime) == 0);

    thread_runtime_stats_t stats =
        thread_runtime_get_stats(runtime);

    assert(stats.submitted_tasks == TASK_COUNT);
    assert(stats.completed_tasks == TASK_COUNT);
    assert(stats.active_tasks == 0U);
    assert(stats.queued_tasks == 0U);

    assert(atomic_load_explicit(&counter,
                                memory_order_relaxed) == TASK_COUNT);

    free(tasks);
    thread_runtime_destroy(runtime);

    puts("high_volume=PASS");
}

int main(void)
{
    test_basic_execution();
    test_bounded_backpressure();
    test_concurrent_producers();
    test_shutdown_rejects_new_work();
    test_high_volume();

    puts("thread_runtime_tests=PASS");
    return 0;
}
