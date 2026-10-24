#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    const char *path;
    const char message[] = "fifo-message";
    pid_t child;

    if (argc != 2) {
        fprintf(stderr, "usage: %s FIFO_PATH\n", argv[0]);
        return 2;
    }

    path = argv[1];
    (void)unlink(path);

    if (mkfifo(path, 0600) != 0) {
        perror("mkfifo");
        return 1;
    }

    child = fork();
    if (child < 0) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        char buffer[64];
        int fd = open(path, O_RDONLY);
        ssize_t n;

        if (fd < 0) {
            perror("open reader");
            return 1;
        }

        n = read(fd, buffer, sizeof buffer - 1U);
        if (n < 0) {
            perror("read");
            return 1;
        }

        buffer[(size_t)n] = '\0';
        printf("reader_received=%s\n", buffer);
        (void)close(fd);
        return 0;
    }

    {
        int fd = open(path, O_WRONLY);
        if (fd < 0) {
            perror("open writer");
            return 1;
        }
        if (write(fd, message, sizeof message - 1U) != (ssize_t)(sizeof message - 1U)) {
            perror("write");
            return 1;
        }
        (void)close(fd);
    }

    if (waitpid(child, NULL, 0) < 0) {
        perror("waitpid");
        return 1;
    }

    {
        struct stat info;
        if (lstat(path, &info) != 0) {
            perror("lstat");
            return 1;
        }
        printf("fifo_type_verified=%s\n", S_ISFIFO(info.st_mode) ? "yes" : "no");
    }

    if (unlink(path) != 0) {
        perror("unlink");
        return 1;
    }

    return 0;
}
