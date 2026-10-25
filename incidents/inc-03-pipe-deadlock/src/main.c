#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int fds[2];
    if (pipe(fds) != 0) return 1;

    if (write(fds[1], "x", 1) != 1) return 1;

    char byte = '\0';
    if (read(fds[0], &byte, 1) != 1) return 1;

    int flags = fcntl(fds[0], F_GETFL);
    if (flags < 0) return 1;
    if (fcntl(fds[0], F_SETFL, flags | O_NONBLOCK) != 0) return 1;

    errno = 0;
    const ssize_t before_close = read(fds[0], &byte, 1);
    const int eagain_seen = before_close == -1 && (errno == EAGAIN || errno == EWOULDBLOCK);

    if (close(fds[1]) != 0) return 1;

    errno = 0;
    const ssize_t after_close = read(fds[0], &byte, 1);

    printf("writer_still_open_no_eof=%s\n", eagain_seen ? "yes" : "no");
    printf("eof_after_writer_close=%s\n", after_close == 0 ? "yes" : "no");

    close(fds[0]);
    return (eagain_seen && after_close == 0) ? 0 : 1;
}
