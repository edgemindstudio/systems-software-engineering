#include <pthread.h>
#include <stdio.h>

static int counter = 0;

static void *worker(void *unused)
{
    (void)unused;
    for (int i = 0; i < 100000; ++i) {
        ++counter;
    }
    return NULL;
}

int main(void)
{
    pthread_t a;
    pthread_t b;

    if (pthread_create(&a, NULL, worker, NULL) != 0) return 1;
    if (pthread_create(&b, NULL, worker, NULL) != 0) return 1;
    if (pthread_join(a, NULL) != 0) return 1;
    if (pthread_join(b, NULL) != 0) return 1;

    printf("final_counter=%d\n", counter);
    return 0;
}
