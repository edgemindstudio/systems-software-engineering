#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static volatile sig_atomic_t received = 0;

static void handle_usr1(int signo)
{
    static const char message[] = "handler_received_SIGUSR1\n";
    (void)signo;
    received = 1;
    (void)write(STDOUT_FILENO, message, sizeof message - 1U);
}

int main(void)
{
    struct sigaction action;
    pid_t child;

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

    while (!received) {
        pause();
    }

    if (waitpid(child, NULL, 0) < 0) {
        perror("waitpid");
        return 1;
    }

    printf("main_observed_signal=yes\n");
    return 0;
}
