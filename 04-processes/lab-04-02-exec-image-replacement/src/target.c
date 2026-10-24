#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    const char *token = getenv("PROCESS_LAB_TOKEN");

    printf("post_exec_pid=%ld\n", (long)getpid());
    printf("argc=%d\n", argc);
    printf("argv1=%s\n", argc > 1 ? argv[1] : "(missing)");
    printf("argv2=%s\n", argc > 2 ? argv[2] : "(missing)");
    printf("env_token=%s\n", token != NULL ? token : "(missing)");
    return 0;
}
