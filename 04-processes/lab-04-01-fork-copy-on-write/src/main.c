#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int value = 41;
    pid_t child;

    printf("before_fork pid=%ld value=%d\n", (long)getpid(), value);
    fflush(stdout);

    child = fork();
    if (child < 0) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        value += 1;
        printf("child pid=%ld ppid=%ld value=%d\n",
               (long)getpid(), (long)getppid(), value);
        fflush(stdout);
        return 0;
    }

    value += 100;
    printf("parent pid=%ld child=%ld value=%d\n",
           (long)getpid(), (long)child, value);

    if (waitpid(child, NULL, 0) < 0) {
        perror("waitpid");
        return 1;
    }

    return 0;
}
