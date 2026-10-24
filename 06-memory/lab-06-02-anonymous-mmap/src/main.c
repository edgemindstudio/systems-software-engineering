#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void)
{
    long page_size = sysconf(_SC_PAGESIZE);
    size_t length;
    char *mapping;

    if (page_size <= 0) {
        fprintf(stderr, "invalid page size\n");
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

    (void)snprintf(mapping, length, "anonymous-mapping-ok");

    printf("page_size=%ld\n", page_size);
    printf("mapping_length=%zu\n", length);
    printf("message=%s\n", mapping);

    if (munmap(mapping, length) != 0) {
        perror("munmap");
        return 1;
    }

    printf("munmap_success=yes\n");
    return 0;
}
