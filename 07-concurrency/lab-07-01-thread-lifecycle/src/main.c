#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct task {
    int id;
    int input;
    int result;
};

static void *worker(void *arg)
{
    struct task *task = arg;
    task->result = task->input * task->input;
    return arg;
}

int main(void)
{
    enum { THREADS = 4 };
    pthread_t threads[THREADS];
    struct task tasks[THREADS];

    for (int i = 0; i < THREADS; ++i) {
        tasks[i].id = i;
        tasks[i].input = i + 2;
        tasks[i].result = 0;
        if (pthread_create(&threads[i], NULL, worker, &tasks[i]) != 0) {
            fprintf(stderr, "pthread_create failed\n");
            return 1;
        }
    }

    for (int i = 0; i < THREADS; ++i) {
        void *returned = NULL;
        if (pthread_join(threads[i], &returned) != 0) {
            fprintf(stderr, "pthread_join failed\n");
            return 1;
        }
        if (returned != &tasks[i]) {
            fprintf(stderr, "unexpected thread return pointer\n");
            return 1;
        }
        printf("thread=%d input=%d result=%d\n",
               tasks[i].id, tasks[i].input, tasks[i].result);
    }

    printf("all_threads_joined=yes\n");
    return 0;
}
