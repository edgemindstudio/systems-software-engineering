#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t child = fork();
    if (child < 0) return 1;

    if (child == 0) {
        _exit(0);
    }

    sleep(1);

    char path[64];
    (void)snprintf(path, sizeof path, "/proc/%ld/stat", (long)child);
    FILE *fp = fopen(path, "r");
    if (fp == NULL) return 1;

    long pid = 0;
    char comm[128];
    char state = '?';

    if (fscanf(fp, "%ld %127s %c", &pid, comm, &state) != 3) {
        fclose(fp);
        return 1;
    }
    fclose(fp);

    printf("child_state_before_wait=%c\n", state);
    printf("zombie_observed=%s\n", state == 'Z' ? "yes" : "no");

    if (waitpid(child, NULL, 0) != child) return 1;

    printf("child_reaped=yes\n");
    return 0;
}
