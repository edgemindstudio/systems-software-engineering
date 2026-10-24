#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    long page_size = sysconf(_SC_PAGESIZE);
    int *shared_virtual_address;
    int sync_pipe[2];
    pid_t child;
    int status = 0;

    if (page_size <= 0) {
        return 1;
    }

    shared_virtual_address = mmap(NULL, (size_t)page_size,
                                  PROT_READ | PROT_WRITE,
                                  MAP_PRIVATE | MAP_ANONYMOUS,
                                  -1, 0);
    if (shared_virtual_address == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    *shared_virtual_address = 100;

    if (pipe(sync_pipe) != 0) {
        perror("pipe");
        return 1;
    }

    child = fork();
    if (child < 0) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        const char done = 'D';
        (void)close(sync_pipe[0]);
        *shared_virtual_address = 200;
        printf("child_value=%d\n", *shared_virtual_address);
        fflush(stdout);
        (void)write(sync_pipe[1], &done, 1U);
        (void)close(sync_pipe[1]);
        _exit(0);
    }

    (void)close(sync_pipe[1]);
    {
        char done = '\0';
        if (read(sync_pipe[0], &done, 1U) != 1 || done != 'D') {
            fprintf(stderr, "child synchronization failed\n");
            return 1;
        }
    }
    (void)close(sync_pipe[0]);

    printf("parent_value_after_child_write=%d\n", *shared_virtual_address);
    *shared_virtual_address = 300;
    printf("parent_value_after_parent_write=%d\n", *shared_virtual_address);

    if (waitpid(child, &status, 0) < 0) {
        perror("waitpid");
        return 1;
    }

    printf("child_exit_success=%s\n",
           WIFEXITED(status) && WEXITSTATUS(status) == 0 ? "yes" : "no");

    (void)munmap(shared_virtual_address, (size_t)page_size);
    return 0;
}
