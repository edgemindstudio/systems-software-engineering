#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t children[3];
    const int codes[3] = {11, 22, 33};

    for (size_t i = 0U; i < 3U; ++i) {
        pid_t child = fork();

        if (child < 0) {
            perror("fork");
            return 1;
        }

        if (child == 0) {
            printf("child index=%zu pid=%ld exit=%d\n",
                   i, (long)getpid(), codes[i]);
            fflush(stdout);
            _exit(codes[i]);
        }

        children[i] = child;
    }

    for (size_t i = 0U; i < 3U; ++i) {
        int status = 0;
        pid_t result = waitpid(children[i], &status, 0);

        if (result < 0) {
            perror("waitpid");
            return 1;
        }

        if (!WIFEXITED(status)) {
            fprintf(stderr, "child did not exit normally\n");
            return 1;
        }

        printf("reaped pid=%ld exit=%d\n",
               (long)result, WEXITSTATUS(status));
    }

    return 0;
}
