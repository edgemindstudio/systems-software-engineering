#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t child;

    printf("root pid=%ld ppid=%ld\n", (long)getpid(), (long)getppid());
    fflush(stdout);

    child = fork();
    if (child < 0) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        pid_t grandchild;

        printf("child pid=%ld ppid=%ld\n", (long)getpid(), (long)getppid());
        fflush(stdout);

        grandchild = fork();
        if (grandchild < 0) {
            perror("fork");
            return 1;
        }

        if (grandchild == 0) {
            printf("grandchild pid=%ld ppid=%ld\n",
                   (long)getpid(), (long)getppid());
            fflush(stdout);
            sleep(1);
            return 0;
        }

        if (waitpid(grandchild, NULL, 0) < 0) {
            perror("waitpid grandchild");
            return 1;
        }
        return 0;
    }

    if (waitpid(child, NULL, 0) < 0) {
        perror("waitpid child");
        return 1;
    }

    return 0;
}
