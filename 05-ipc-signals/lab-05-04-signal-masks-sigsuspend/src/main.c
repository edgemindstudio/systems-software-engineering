#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static volatile sig_atomic_t observed = 0;

static void handle_usr1(int signo)
{
    (void)signo;
    observed = 1;
}

int main(void)
{
    sigset_t block;
    sigset_t oldmask;
    sigset_t pending;
    struct sigaction action;
    pid_t child;

    sigemptyset(&block);
    sigaddset(&block, SIGUSR1);

    if (sigprocmask(SIG_BLOCK, &block, &oldmask) != 0) {
        perror("sigprocmask");
        return 1;
    }

    action.sa_handler = handle_usr1;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGUSR1, &action, NULL) != 0) {
        perror("sigaction");
        return 1;
    }

    child = fork();
    if (child < 0) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        if (kill(getppid(), SIGUSR1) != 0) {
            perror("kill");
            return 1;
        }
        return 0;
    }

    if (waitpid(child, NULL, 0) < 0) {
        perror("waitpid");
        return 1;
    }

    if (sigpending(&pending) != 0) {
        perror("sigpending");
        return 1;
    }

    printf("pending_before_sigsuspend=%s\n",
           sigismember(&pending, SIGUSR1) == 1 ? "yes" : "no");

    while (!observed) {
        (void)sigsuspend(&oldmask);
    }

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) != 0) {
        perror("sigprocmask restore");
        return 1;
    }

    printf("handler_observed_after_unblock=yes\n");
    return 0;
}
