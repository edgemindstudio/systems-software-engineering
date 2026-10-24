#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void)
{
    long page_size = sysconf(_SC_PAGESIZE);
    unsigned char *mapping;
    size_t length;

    if (page_size <= 0) {
        return 1;
    }

    length = (size_t)page_size * 4U;
    mapping = mmap(NULL, length,
                   PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS,
                   -1, 0);
    if (mapping == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    for (size_t i = 0U; i < 4U; ++i) {
        mapping[i * (size_t)page_size] = (unsigned char)(0x40U + i);
    }

    printf("before=%u,%u,%u,%u\n",
           (unsigned int)mapping[0],
           (unsigned int)mapping[(size_t)page_size],
           (unsigned int)mapping[(size_t)page_size * 2U],
           (unsigned int)mapping[(size_t)page_size * 3U]);

    if (madvise(mapping, length, MADV_DONTNEED) != 0) {
        perror("madvise");
        return 1;
    }

    printf("after=%u,%u,%u,%u\n",
           (unsigned int)mapping[0],
           (unsigned int)mapping[(size_t)page_size],
           (unsigned int)mapping[(size_t)page_size * 2U],
           (unsigned int)mapping[(size_t)page_size * 3U]);

    (void)munmap(mapping, length);
    return 0;
}
