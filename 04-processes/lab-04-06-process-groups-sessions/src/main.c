#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static void print_ids(const char *label)
{
    printf("%s pid=%ld ppid=%ld pgid=%ld sid=%ld\n",
           label,
           (long)getpid(),
           (long)getppid(),
           (long)getpgrp(),
           (long)getsid(0));
    fflush(stdout);
}

int main(void)
{
    pid_t group_child;
    pid_t session_child;

    print_ids("parent");

    group_child = fork();
    if (group_child < 0) {
        perror("fork group child");
        return 1;
    }

    if (group_child == 0) {
        if (setpgid(0, 0) != 0) {
            perror("setpgid");
            return 1;
        }
        print_ids("group_child");
        return 0;
    }

    if (waitpid(group_child, NULL, 0) < 0) {
        perror("waitpid group child");
        return 1;
    }

    session_child = fork();
    if (session_child < 0) {
        perror("fork session child");
        return 1;
    }

    if (session_child == 0) {
        if (setsid() < 0) {
            perror("setsid");
            return 1;
        }
        print_ids("session_child");
        return 0;
    }

    if (waitpid(session_child, NULL, 0) < 0) {
        perror("waitpid session child");
        return 1;
    }

    return 0;
}
