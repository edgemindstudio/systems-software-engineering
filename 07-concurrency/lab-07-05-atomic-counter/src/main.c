#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>

enum { THREADS = 8, INCREMENTS = 100000 };

static _Atomic long counter = 0;

static void *worker(void *arg)
{
    (void)arg;

    for (int i = 0; i < INCREMENTS; ++i) {
        (void)atomic_fetch_add_explicit(&counter, 1L, memory_order_relaxed);
    }

    return NULL;
}

int main(void)
{
    pthread_t threads[THREADS];
    long expected = (long)THREADS * (long)INCREMENTS;

    for (int i = 0; i < THREADS; ++i) {
        if (pthread_create(&threads[i], NULL, worker, NULL) != 0) {
            return 1;
        }
    }

    for (int i = 0; i < THREADS; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            return 1;
        }
    }

    {
        long actual = atomic_load_explicit(&counter, memory_order_relaxed);
        printf("expected=%ld\n", expected);
        printf("actual=%ld\n", actual);
        printf("atomic_counter_correct=%s\n",
               actual == expected ? "yes" : "no");
    }

    return 0;
}
