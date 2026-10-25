#include "thread_runtime/thread_runtime.h"

#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    thread_runtime_task_fn function;
    void *context;
} task_t;

struct thread_runtime {
    pthread_t *workers;
    size_t worker_count;

    task_t *queue;
    size_t queue_capacity;
    size_t queue_head;
    size_t queue_tail;
    size_t queue_size;

    size_t active_tasks;
    size_t submitted_tasks;
    size_t completed_tasks;
    size_t rejected_tasks;

    int accepting_tasks;
    int stopping;

    pthread_mutex_t mutex;
    pthread_cond_t work_available;
    pthread_cond_t queue_space;
    pthread_cond_t idle;
};

static int runtime_is_idle(const thread_runtime_t *runtime)
{
    return runtime->queue_size == 0U && runtime->active_tasks == 0U;
}

static void *worker_main(void *context)
{
    thread_runtime_t *runtime = context;

    for (;;) {
        task_t task = {0};

        if (pthread_mutex_lock(&runtime->mutex) != 0) {
            return NULL;
        }

        while (runtime->queue_size == 0U && runtime->stopping == 0) {
            (void)pthread_cond_wait(&runtime->work_available,
                                    &runtime->mutex);
        }

        if (runtime->queue_size == 0U && runtime->stopping != 0) {
            (void)pthread_mutex_unlock(&runtime->mutex);
            break;
        }

        task = runtime->queue[runtime->queue_head];
        runtime->queue_head =
            (runtime->queue_head + 1U) % runtime->queue_capacity;
        --runtime->queue_size;
        ++runtime->active_tasks;

        (void)pthread_cond_signal(&runtime->queue_space);
        (void)pthread_mutex_unlock(&runtime->mutex);

        task.function(task.context);

        (void)pthread_mutex_lock(&runtime->mutex);
        --runtime->active_tasks;
        ++runtime->completed_tasks;

        if (runtime_is_idle(runtime)) {
            (void)pthread_cond_broadcast(&runtime->idle);
        }

        (void)pthread_mutex_unlock(&runtime->mutex);
    }

    return NULL;
}

thread_runtime_t *thread_runtime_create(size_t worker_count,
                                        size_t queue_capacity)
{
    if (worker_count == 0U || queue_capacity == 0U) {
        errno = EINVAL;
        return NULL;
    }

    thread_runtime_t *runtime = calloc(1U, sizeof *runtime);
    if (runtime == NULL) {
        return NULL;
    }

    runtime->workers = calloc(worker_count, sizeof *runtime->workers);
    runtime->queue = calloc(queue_capacity, sizeof *runtime->queue);

    if (runtime->workers == NULL || runtime->queue == NULL) {
        free(runtime->queue);
        free(runtime->workers);
        free(runtime);
        return NULL;
    }

    runtime->worker_count = worker_count;
    runtime->queue_capacity = queue_capacity;
    runtime->accepting_tasks = 1;

    if (pthread_mutex_init(&runtime->mutex, NULL) != 0 ||
        pthread_cond_init(&runtime->work_available, NULL) != 0 ||
        pthread_cond_init(&runtime->queue_space, NULL) != 0 ||
        pthread_cond_init(&runtime->idle, NULL) != 0) {
        free(runtime->queue);
        free(runtime->workers);
        free(runtime);
        return NULL;
    }

    size_t started = 0U;
    for (; started < worker_count; ++started) {
        if (pthread_create(&runtime->workers[started],
                           NULL,
                           worker_main,
                           runtime) != 0) {
            (void)pthread_mutex_lock(&runtime->mutex);
            runtime->stopping = 1;
            runtime->accepting_tasks = 0;
            (void)pthread_cond_broadcast(&runtime->work_available);
            (void)pthread_mutex_unlock(&runtime->mutex);

            for (size_t i = 0U; i < started; ++i) {
                (void)pthread_join(runtime->workers[i], NULL);
            }

            (void)pthread_cond_destroy(&runtime->idle);
            (void)pthread_cond_destroy(&runtime->queue_space);
            (void)pthread_cond_destroy(&runtime->work_available);
            (void)pthread_mutex_destroy(&runtime->mutex);
            free(runtime->queue);
            free(runtime->workers);
            free(runtime);
            return NULL;
        }
    }

    return runtime;
}

int thread_runtime_submit(thread_runtime_t *runtime,
                          thread_runtime_task_fn function,
                          void *context)
{
    if (runtime == NULL || function == NULL) {
        return EINVAL;
    }

    int rc = pthread_mutex_lock(&runtime->mutex);
    if (rc != 0) {
        return rc;
    }

    while (runtime->queue_size == runtime->queue_capacity &&
           runtime->accepting_tasks != 0) {
        rc = pthread_cond_wait(&runtime->queue_space, &runtime->mutex);
        if (rc != 0) {
            (void)pthread_mutex_unlock(&runtime->mutex);
            return rc;
        }
    }

    if (runtime->accepting_tasks == 0) {
        ++runtime->rejected_tasks;
        (void)pthread_mutex_unlock(&runtime->mutex);
        return ECANCELED;
    }

    runtime->queue[runtime->queue_tail].function = function;
    runtime->queue[runtime->queue_tail].context = context;
    runtime->queue_tail =
        (runtime->queue_tail + 1U) % runtime->queue_capacity;
    ++runtime->queue_size;
    ++runtime->submitted_tasks;

    (void)pthread_cond_signal(&runtime->work_available);
    (void)pthread_mutex_unlock(&runtime->mutex);
    return 0;
}

int thread_runtime_wait_idle(thread_runtime_t *runtime)
{
    if (runtime == NULL) {
        return EINVAL;
    }

    int rc = pthread_mutex_lock(&runtime->mutex);
    if (rc != 0) {
        return rc;
    }

    while (!runtime_is_idle(runtime)) {
        rc = pthread_cond_wait(&runtime->idle, &runtime->mutex);
        if (rc != 0) {
            (void)pthread_mutex_unlock(&runtime->mutex);
            return rc;
        }
    }

    (void)pthread_mutex_unlock(&runtime->mutex);
    return 0;
}

thread_runtime_stats_t
thread_runtime_get_stats(thread_runtime_t *runtime)
{
    thread_runtime_stats_t stats = {0};

    if (runtime == NULL) {
        return stats;
    }

    (void)pthread_mutex_lock(&runtime->mutex);

    stats.worker_count = runtime->worker_count;
    stats.queue_capacity = runtime->queue_capacity;
    stats.queued_tasks = runtime->queue_size;
    stats.active_tasks = runtime->active_tasks;
    stats.submitted_tasks = runtime->submitted_tasks;
    stats.completed_tasks = runtime->completed_tasks;
    stats.rejected_tasks = runtime->rejected_tasks;
    stats.accepting_tasks = runtime->accepting_tasks;

    (void)pthread_mutex_unlock(&runtime->mutex);
    return stats;
}

void thread_runtime_shutdown(thread_runtime_t *runtime)
{
    if (runtime == NULL) {
        return;
    }

    (void)pthread_mutex_lock(&runtime->mutex);

    if (runtime->stopping != 0) {
        (void)pthread_mutex_unlock(&runtime->mutex);
        return;
    }

    runtime->accepting_tasks = 0;
    runtime->stopping = 1;

    (void)pthread_cond_broadcast(&runtime->work_available);
    (void)pthread_cond_broadcast(&runtime->queue_space);
    (void)pthread_mutex_unlock(&runtime->mutex);

    for (size_t i = 0U; i < runtime->worker_count; ++i) {
        (void)pthread_join(runtime->workers[i], NULL);
    }
}

void thread_runtime_destroy(thread_runtime_t *runtime)
{
    if (runtime == NULL) {
        return;
    }

    thread_runtime_shutdown(runtime);

    (void)pthread_cond_destroy(&runtime->idle);
    (void)pthread_cond_destroy(&runtime->queue_space);
    (void)pthread_cond_destroy(&runtime->work_available);
    (void)pthread_mutex_destroy(&runtime->mutex);

    free(runtime->queue);
    free(runtime->workers);
    free(runtime);
}
