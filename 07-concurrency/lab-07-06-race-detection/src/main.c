#include <pthread.h>
#include <stdio.h>

enum { THREADS = 4, INCREMENTS = 100000 };

static volatile long counter = 0;

static void *worker(void *arg)
{
    (void)arg;

    for (int i = 0; i < INCREMENTS; ++i) {
        ++counter;
    }

    return NULL;
}

int main(void)
{
    pthread_t threads[THREADS];

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

    printf("expected=%ld\n", (long)THREADS * (long)INCREMENTS);
    printf("observed=%ld\n", counter);
    printf("intentional_race_present=yes\n");
    return 0;
}
