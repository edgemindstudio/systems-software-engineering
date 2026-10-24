#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

int main(void)
{
    pid_t child = fork();

    if (child < 0) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        char *const argv[] = {
            (char *)"exec_target",
            (char *)"alpha",
            (char *)"beta",
            NULL
        };

        if (setenv("PROCESS_LAB_TOKEN", "exec-preserved-env", 1) != 0) {
            perror("setenv");
            return 1;
        }

        printf("pre_exec_pid=%ld\n", (long)getpid());
        fflush(stdout);

        execve("./build/exec_target", argv, environ);
        perror("execve");
        return 127;
    }

    {
        int status = 0;
        if (waitpid(child, &status, 0) < 0) {
            perror("waitpid");
            return 1;
        }
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            fprintf(stderr, "child exec path failed\n");
            return 1;
        }
    }

    return 0;
}
