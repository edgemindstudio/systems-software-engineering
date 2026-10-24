#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int fd_is_open(int fd)
{
    errno = 0;
    if (fcntl(fd, F_GETFD) >= 0) {
        return 1;
    }
    return errno != EBADF;
}

int main(int argc, char **argv)
{
    char cwd[4096];
    const char *token;
    int inherited_fd;
    int cloexec_fd;

    if (argc != 3) {
        return 2;
    }

    inherited_fd = atoi(argv[1]);
    cloexec_fd = atoi(argv[2]);
    token = getenv("PROCESS_INHERIT_TOKEN");

    if (getcwd(cwd, sizeof cwd) == NULL) {
        perror("getcwd");
        return 1;
    }

    printf("cwd=%s\n", cwd);
    printf("env_token=%s\n", token != NULL ? token : "(missing)");
    printf("inherited_fd_open=%s\n", fd_is_open(inherited_fd) ? "yes" : "no");
    printf("cloexec_fd_open=%s\n", fd_is_open(cloexec_fd) ? "yes" : "no");

    return 0;
}
