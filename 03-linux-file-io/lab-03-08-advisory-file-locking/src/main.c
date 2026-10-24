#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static int try_lock(int fd)
{
    struct flock lock;

    memset(&lock, 0, sizeof lock);
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    return fcntl(fd, F_SETLK, &lock);
}

int main(int argc, char **argv)
{
    int fd;
    int hold_seconds = 0;

    if (argc < 3 || argc > 4) {
        fprintf(stderr, "usage: %s FILE try|hold [SECONDS]\n", argv[0]);
        return 2;
    }

    if (argc == 4) {
        hold_seconds = atoi(argv[3]);
    }

    fd = open(argv[1], O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    if (try_lock(fd) != 0) {
        if (errno == EACCES || errno == EAGAIN) {
            printf("lock=busy\n");
            (void)close(fd);
            return 3;
        }

        fprintf(stderr, "fcntl: %s\n", strerror(errno));
        (void)close(fd);
        return 1;
    }

    printf("lock=acquired pid=%ld\n", (long)getpid());
    fflush(stdout);

    if (strcmp(argv[2], "hold") == 0 && hold_seconds > 0) {
        struct timespec delay;
        delay.tv_sec = hold_seconds;
        delay.tv_nsec = 0L;

        while (nanosleep(&delay, &delay) != 0 && errno == EINTR) {
        }
    }

    (void)close(fd);
    return 0;
}
