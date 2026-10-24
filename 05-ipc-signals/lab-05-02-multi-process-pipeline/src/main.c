#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int first[2];
    int second[2];
    pid_t sort_child;
    pid_t uniq_child;
    const char input[] = "alpha\nbeta\nalpha\n";

    if (pipe(first) != 0 || pipe(second) != 0) {
        perror("pipe");
        return 1;
    }

    sort_child = fork();
    if (sort_child < 0) {
        perror("fork sort");
        return 1;
    }

    if (sort_child == 0) {
        (void)dup2(first[0], STDIN_FILENO);
        (void)dup2(second[1], STDOUT_FILENO);
        (void)close(first[0]);
        (void)close(first[1]);
        (void)close(second[0]);
        (void)close(second[1]);
        execlp("sort", "sort", (char *)NULL);
        perror("execlp sort");
        _exit(127);
    }

    uniq_child = fork();
    if (uniq_child < 0) {
        perror("fork uniq");
        return 1;
    }

    if (uniq_child == 0) {
        (void)dup2(second[0], STDIN_FILENO);
        (void)close(first[0]);
        (void)close(first[1]);
        (void)close(second[0]);
        (void)close(second[1]);
        execlp("uniq", "uniq", "-c", (char *)NULL);
        perror("execlp uniq");
        _exit(127);
    }

    (void)close(first[0]);
    (void)close(second[0]);
    (void)close(second[1]);

    if (write(first[1], input, sizeof input - 1U) != (ssize_t)(sizeof input - 1U)) {
        perror("write");
        return 1;
    }
    (void)close(first[1]);

    {
        int status1 = 0;
        int status2 = 0;

        if (waitpid(sort_child, &status1, 0) < 0 ||
            waitpid(uniq_child, &status2, 0) < 0) {
            perror("waitpid");
            return 1;
        }

        if (!WIFEXITED(status1) || WEXITSTATUS(status1) != 0 ||
            !WIFEXITED(status2) || WEXITSTATUS(status2) != 0) {
            fprintf(stderr, "pipeline child failed\n");
            return 1;
        }
    }

    return 0;
}
