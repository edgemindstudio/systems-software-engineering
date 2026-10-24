#include <signal.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    long page_size = sysconf(_SC_PAGESIZE);
    size_t length;
    unsigned char *mapping;
    pid_t child;
    int status = 0;

    if (page_size <= 0) {
        return 1;
    }

    length = (size_t)page_size * 2U;
    mapping = mmap(NULL, length,
                   PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS,
                   -1, 0);
    if (mapping == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    mapping[0] = 0x5aU;

    if (mprotect(mapping + (size_t)page_size,
                 (size_t)page_size, PROT_NONE) != 0) {
        perror("mprotect");
        return 1;
    }

    child = fork();
    if (child < 0) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        volatile unsigned char value = mapping[(size_t)page_size];
        (void)value;
        _exit(0);
    }

    if (waitpid(child, &status, 0) < 0) {
        perror("waitpid");
        return 1;
    }

    printf("first_page_value=%u\n", (unsigned int)mapping[0]);
    printf("guard_child_signaled=%s\n", WIFSIGNALED(status) ? "yes" : "no");
    printf("guard_signal=%d\n", WIFSIGNALED(status) ? WTERMSIG(status) : 0);
    printf("expected_sigsegv=%s\n",
           WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV ? "yes" : "no");

    if (munmap(mapping, length) != 0) {
        perror("munmap");
        return 1;
    }

    return 0;
}
