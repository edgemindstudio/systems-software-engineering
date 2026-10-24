#include <pthread.h>
#include <stdio.h>

enum { CAPACITY = 8, ITEMS = 100 };

struct queue {
    int data[CAPACITY];
    int head;
    int tail;
    int count;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
};

static struct queue q = {
    .data = {0},
    .head = 0,
    .tail = 0,
    .count = 0,
    .lock = PTHREAD_MUTEX_INITIALIZER,
    .not_empty = PTHREAD_COND_INITIALIZER,
    .not_full = PTHREAD_COND_INITIALIZER
};

static void *producer(void *arg)
{
    (void)arg;

    for (int value = 1; value <= ITEMS; ++value) {
        (void)pthread_mutex_lock(&q.lock);
        while (q.count == CAPACITY) {
            (void)pthread_cond_wait(&q.not_full, &q.lock);
        }

        q.data[q.tail] = value;
        q.tail = (q.tail + 1) % CAPACITY;
        ++q.count;

        (void)pthread_cond_signal(&q.not_empty);
        (void)pthread_mutex_unlock(&q.lock);
    }

    return NULL;
}

static void *consumer(void *arg)
{
    long *sum = arg;

    for (int i = 0; i < ITEMS; ++i) {
        int value;

        (void)pthread_mutex_lock(&q.lock);
        while (q.count == 0) {
            (void)pthread_cond_wait(&q.not_empty, &q.lock);
        }

        value = q.data[q.head];
        q.head = (q.head + 1) % CAPACITY;
        --q.count;

        (void)pthread_cond_signal(&q.not_full);
        (void)pthread_mutex_unlock(&q.lock);

        *sum += value;
    }

    return NULL;
}

int main(void)
{
    pthread_t producer_thread;
    pthread_t consumer_thread;
    long sum = 0;
    long expected = ((long)ITEMS * ((long)ITEMS + 1L)) / 2L;

    if (pthread_create(&producer_thread, NULL, producer, NULL) != 0 ||
        pthread_create(&consumer_thread, NULL, consumer, &sum) != 0) {
        return 1;
    }

    if (pthread_join(producer_thread, NULL) != 0 ||
        pthread_join(consumer_thread, NULL) != 0) {
        return 1;
    }

    printf("expected_sum=%ld\n", expected);
    printf("actual_sum=%ld\n", sum);
    printf("queue_empty=%s\n", q.count == 0 ? "yes" : "no");
    printf("producer_consumer_correct=%s\n",
           sum == expected && q.count == 0 ? "yes" : "no");
    return 0;
}
