#include <pthread.h>
#include <stdio.h>

enum { WORKERS = 4, QUEUE_CAP = 8, TASKS = 40 };

struct pool {
    int queue[QUEUE_CAP];
    int head;
    int tail;
    int count;
    int shutdown;
    int completed;
    long sum;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
};

static struct pool pool = {
    .queue = {0},
    .head = 0,
    .tail = 0,
    .count = 0,
    .shutdown = 0,
    .completed = 0,
    .sum = 0,
    .lock = PTHREAD_MUTEX_INITIALIZER,
    .not_empty = PTHREAD_COND_INITIALIZER,
    .not_full = PTHREAD_COND_INITIALIZER
};

static void *worker(void *arg)
{
    (void)arg;

    for (;;) {
        int task;

        (void)pthread_mutex_lock(&pool.lock);

        while (pool.count == 0 && !pool.shutdown) {
            (void)pthread_cond_wait(&pool.not_empty, &pool.lock);
        }

        if (pool.count == 0 && pool.shutdown) {
            (void)pthread_mutex_unlock(&pool.lock);
            break;
        }

        task = pool.queue[pool.head];
        pool.head = (pool.head + 1) % QUEUE_CAP;
        --pool.count;
        (void)pthread_cond_signal(&pool.not_full);
        (void)pthread_mutex_unlock(&pool.lock);

        {
            long result = (long)task * (long)task;
            (void)pthread_mutex_lock(&pool.lock);
            pool.sum += result;
            ++pool.completed;
            (void)pthread_mutex_unlock(&pool.lock);
        }
    }

    return NULL;
}

int main(void)
{
    pthread_t workers[WORKERS];
    long expected = 0;

    for (int i = 0; i < WORKERS; ++i) {
        if (pthread_create(&workers[i], NULL, worker, NULL) != 0) {
            return 1;
        }
    }

    for (int task = 1; task <= TASKS; ++task) {
        expected += (long)task * (long)task;

        (void)pthread_mutex_lock(&pool.lock);
        while (pool.count == QUEUE_CAP) {
            (void)pthread_cond_wait(&pool.not_full, &pool.lock);
        }

        pool.queue[pool.tail] = task;
        pool.tail = (pool.tail + 1) % QUEUE_CAP;
        ++pool.count;

        (void)pthread_cond_signal(&pool.not_empty);
        (void)pthread_mutex_unlock(&pool.lock);
    }

    (void)pthread_mutex_lock(&pool.lock);
    pool.shutdown = 1;
    (void)pthread_cond_broadcast(&pool.not_empty);
    (void)pthread_mutex_unlock(&pool.lock);

    for (int i = 0; i < WORKERS; ++i) {
        if (pthread_join(workers[i], NULL) != 0) {
            return 1;
        }
    }

    printf("tasks_submitted=%d\n", TASKS);
    printf("tasks_completed=%d\n", pool.completed);
    printf("expected_sum=%ld\n", expected);
    printf("actual_sum=%ld\n", pool.sum);
    printf("worker_pool_correct=%s\n",
           pool.completed == TASKS && pool.sum == expected ? "yes" : "no");
    return 0;
}
