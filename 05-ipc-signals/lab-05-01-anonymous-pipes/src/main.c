#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int write_all(int fd, const char *buffer, size_t length)
{
    size_t total = 0U;

    while (total < length) {
        ssize_t n = write(fd, buffer + total, length - total);
        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        total += (size_t)n;
    }

    return 0;
}

int main(void)
{
    int fds[2];
    pid_t child;
    const char message[] = "hello-through-pipe";
    char buffer[64];
    size_t used = 0U;

    if (pipe(fds) != 0) {
        perror("pipe");
        return 1;
    }

    child = fork();
    if (child < 0) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        (void)close(fds[1]);

        for (;;) {
            ssize_t n = read(fds[0], buffer + used, sizeof buffer - 1U - used);
            if (n < 0) {
                if (errno == EINTR) {
                    continue;
                }
                perror("read");
                return 1;
            }
            if (n == 0) {
                break;
            }
            used += (size_t)n;
        }

        buffer[used] = '\0';
        printf("child_received=%s\n", buffer);
        printf("eof_after_writer_close=yes\n");
        (void)close(fds[0]);
        return 0;
    }

    (void)close(fds[0]);
    if (write_all(fds[1], message, sizeof message - 1U) != 0) {
        perror("write");
        return 1;
    }
    (void)close(fds[1]);

    if (waitpid(child, NULL, 0) < 0) {
        perror("waitpid");
        return 1;
    }

    return 0;
}
