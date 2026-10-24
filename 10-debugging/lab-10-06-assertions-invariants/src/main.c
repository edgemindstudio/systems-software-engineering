#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int divide_positive(int numerator, int denominator)
{
    assert(denominator > 0);
    return numerator / denominator;
}

int main(void)
{
    pid_t child = fork();
    int status = 0;

    if (child < 0) {
        return 1;
    }

    if (child == 0) {
        (void)divide_positive(10, 0);
        _exit(0);
    }

    if (waitpid(child, &status, 0) < 0) {
        return 1;
    }

    printf("child_signaled=%s\n", WIFSIGNALED(status) ? "yes" : "no");
    printf("signal=%d\n", WIFSIGNALED(status) ? WTERMSIG(status) : 0);
    printf("assertion_failure_isolated=%s\n",
           WIFSIGNALED(status) ? "yes" : "no");
    return 0;
}
