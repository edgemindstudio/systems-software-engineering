#include <pthread.h>
#include <stdio.h>

enum { THREADS = 8, INCREMENTS = 50000 };

static long counter = 0;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static void *worker(void *arg)
{
    (void)arg;

    for (int i = 0; i < INCREMENTS; ++i) {
        if (pthread_mutex_lock(&lock) != 0) {
            return (void *)1;
        }
        ++counter;
        if (pthread_mutex_unlock(&lock) != 0) {
            return (void *)1;
        }
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
        void *result = NULL;
        if (pthread_join(threads[i], &result) != 0 || result != NULL) {
            return 1;
        }
    }

    printf("expected=%ld\n", (long)THREADS * (long)INCREMENTS);
    printf("actual=%ld\n", counter);
    printf("mutex_counter_correct=%s\n",
           counter == (long)THREADS * (long)INCREMENTS ? "yes" : "no");
    return 0;
}
