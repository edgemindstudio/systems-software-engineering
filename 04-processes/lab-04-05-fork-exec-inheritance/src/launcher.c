#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

int main(void)
{
    int inherited_fd;
    int cloexec_fd;
    pid_t child;

    if (mkdir("workspace", 0755) != 0) {
        /* Workspace may already exist from validation setup. */
    }

    inherited_fd = open("workspace/inherited.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (inherited_fd < 0) {
        perror("open inherited");
        return 1;
    }

    cloexec_fd = open("workspace/cloexec.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (cloexec_fd < 0) {
        perror("open cloexec");
        return 1;
    }

    if (fcntl(cloexec_fd, F_SETFD, FD_CLOEXEC) < 0) {
        perror("fcntl");
        return 1;
    }

    if (setenv("PROCESS_INHERIT_TOKEN", "visible-after-exec", 1) != 0) {
        perror("setenv");
        return 1;
    }

    child = fork();
    if (child < 0) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        char inherited_arg[32];
        char cloexec_arg[32];
        char *const argv[] = {
            (char *)"inherit_target",
            inherited_arg,
            cloexec_arg,
            NULL
        };

        (void)snprintf(inherited_arg, sizeof inherited_arg, "%d", inherited_fd);
        (void)snprintf(cloexec_arg, sizeof cloexec_arg, "%d", cloexec_fd);

        execve("./build/inherit_target", argv, environ);
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
            return 1;
        }
    }

    (void)close(cloexec_fd);
    (void)close(inherited_fd);
    return 0;
}
