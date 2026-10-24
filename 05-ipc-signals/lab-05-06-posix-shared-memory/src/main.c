#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct shared_state {
    int ready;
    int value;
    char message[64];
};

int main(void)
{
    char name[64];
    int fd;
    int notify[2];
    struct shared_state *state;
    pid_t child;

    (void)snprintf(name, sizeof name, "/sse_ipc_lab_%ld", (long)getpid());

    fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd < 0) {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(fd, (off_t)sizeof(struct shared_state)) != 0) {
        perror("ftruncate");
        return 1;
    }

    state = mmap(NULL, sizeof *state,
                 PROT_READ | PROT_WRITE,
                 MAP_SHARED, fd, 0);
    if (state == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    memset(state, 0, sizeof *state);

    if (pipe(notify) != 0) {
        perror("pipe");
        return 1;
    }

    child = fork();
    if (child < 0) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        const char token = 'R';
        (void)close(notify[0]);
        state->value = 2026;
        (void)snprintf(state->message, sizeof state->message,
                       "shared-memory-visible");
        state->ready = 1;
        if (write(notify[1], &token, 1U) != 1) {
            _exit(2);
        }
        (void)close(notify[1]);
        _exit(0);
    }

    (void)close(notify[1]);

    {
        char token = '\0';
        if (read(notify[0], &token, 1U) != 1 || token != 'R') {
            fprintf(stderr, "notification failed\n");
            return 1;
        }
    }

    (void)close(notify[0]);

    printf("shared_ready=%d\n", state->ready);
    printf("shared_value=%d\n", state->value);
    printf("shared_message=%s\n", state->message);

    if (waitpid(child, NULL, 0) < 0) {
        perror("waitpid");
        return 1;
    }

    if (munmap(state, sizeof *state) != 0) {
        perror("munmap");
        return 1;
    }
    (void)close(fd);

    if (shm_unlink(name) != 0) {
        perror("shm_unlink");
        return 1;
    }

    printf("shared_memory_unlinked=yes\n");
    return 0;
}
