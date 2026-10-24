#include <pthread.h>
#include <stdio.h>

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int ready = 0;
static int observed = 0;

static void *waiter(void *arg)
{
    (void)arg;

    if (pthread_mutex_lock(&lock) != 0) {
        return (void *)1;
    }

    while (!ready) {
        if (pthread_cond_wait(&cond, &lock) != 0) {
            (void)pthread_mutex_unlock(&lock);
            return (void *)1;
        }
    }

    observed = 1;
    printf("waiter_observed_ready=yes\n");

    if (pthread_mutex_unlock(&lock) != 0) {
        return (void *)1;
    }

    return NULL;
}

int main(void)
{
    pthread_t thread;

    if (pthread_create(&thread, NULL, waiter, NULL) != 0) {
        return 1;
    }


    if (pthread_mutex_lock(&lock) != 0) {
        return 1;
    }

    ready = 1;
    if (pthread_cond_signal(&cond) != 0) {
        return 1;
    }

    if (pthread_mutex_unlock(&lock) != 0) {
        return 1;
    }

    if (pthread_join(thread, NULL) != 0) {
        return 1;
    }

    printf("predicate_ready=%d\n", ready);
    printf("waiter_completed=%s\n", observed ? "yes" : "no");
    return 0;
}
