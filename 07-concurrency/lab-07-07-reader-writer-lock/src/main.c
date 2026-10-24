#include <pthread.h>
#include <stdio.h>

enum { READERS = 4, WRITES = 1000, READ_LOOPS = 2000 };

static pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
static long value = 0;

static void *writer(void *arg)
{
    (void)arg;

    for (int i = 0; i < WRITES; ++i) {
        (void)pthread_rwlock_wrlock(&rwlock);
        ++value;
        (void)pthread_rwlock_unlock(&rwlock);
    }

    return NULL;
}

static void *reader(void *arg)
{
    long *max_seen = arg;

    for (int i = 0; i < READ_LOOPS; ++i) {
        long snapshot;
        (void)pthread_rwlock_rdlock(&rwlock);
        snapshot = value;
        (void)pthread_rwlock_unlock(&rwlock);
        if (snapshot > *max_seen) {
            *max_seen = snapshot;
        }
    }

    return NULL;
}

int main(void)
{
    pthread_t writer_thread;
    pthread_t readers[READERS];
    long seen[READERS] = {0};

    if (pthread_create(&writer_thread, NULL, writer, NULL) != 0) {
        return 1;
    }

    for (int i = 0; i < READERS; ++i) {
        if (pthread_create(&readers[i], NULL, reader, &seen[i]) != 0) {
            return 1;
        }
    }

    if (pthread_join(writer_thread, NULL) != 0) {
        return 1;
    }

    for (int i = 0; i < READERS; ++i) {
        if (pthread_join(readers[i], NULL) != 0) {
            return 1;
        }
    }

    printf("final_value=%ld\n", value);
    printf("expected_final=%d\n", WRITES);
    printf("rwlock_writer_correct=%s\n", value == WRITES ? "yes" : "no");
    return 0;
}
