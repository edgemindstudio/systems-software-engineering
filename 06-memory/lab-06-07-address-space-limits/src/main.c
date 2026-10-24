#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t child = fork();
    int status = 0;

    if (child < 0) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        struct rlimit limit;
        void *memory;

        limit.rlim_cur = 64U * 1024U * 1024U;
        limit.rlim_max = 64U * 1024U * 1024U;

        if (setrlimit(RLIMIT_AS, &limit) != 0) {
            perror("setrlimit");
            _exit(2);
        }

        memory = malloc(256U * 1024U * 1024U);
        if (memory == NULL) {
            printf("allocation_failed_under_limit=yes\n");
            fflush(stdout);
            _exit(0);
        }

        free(memory);
        printf("allocation_failed_under_limit=no\n");
        fflush(stdout);
        _exit(3);
    }

    if (waitpid(child, &status, 0) < 0) {
        perror("waitpid");
        return 1;
    }

    printf("child_exit_status=%d\n",
           WIFEXITED(status) ? WEXITSTATUS(status) : -1);
    printf("controlled_failure_verified=%s\n",
           WIFEXITED(status) && WEXITSTATUS(status) == 0 ? "yes" : "no");
    return 0;
}
