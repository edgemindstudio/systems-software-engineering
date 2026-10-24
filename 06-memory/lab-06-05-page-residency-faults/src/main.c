#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>

static size_t count_resident(const unsigned char *vec, size_t pages)
{
    size_t count = 0U;
    for (size_t i = 0U; i < pages; ++i) {
        if ((vec[i] & 1U) != 0U) {
            ++count;
        }
    }
    return count;
}

int main(void)
{
    const size_t pages = 64U;
    long page_size = sysconf(_SC_PAGESIZE);
    size_t length;
    unsigned char *mapping;
    unsigned char *vec;
    struct rusage before;
    struct rusage after;

    if (page_size <= 0) {
        return 1;
    }

    length = (size_t)page_size * pages;
    mapping = mmap(NULL, length,
                   PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS,
                   -1, 0);
    if (mapping == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    vec = calloc(pages, sizeof *vec);
    if (vec == NULL) {
        perror("calloc");
        return 1;
    }

    if (mincore(mapping, length, vec) != 0) {
        perror("mincore before");
        return 1;
    }
    printf("resident_before=%zu\n", count_resident(vec, pages));

    if (getrusage(RUSAGE_SELF, &before) != 0) {
        perror("getrusage before");
        return 1;
    }

    for (size_t i = 0U; i < pages; ++i) {
        mapping[i * (size_t)page_size] = (unsigned char)i;
    }

    if (getrusage(RUSAGE_SELF, &after) != 0) {
        perror("getrusage after");
        return 1;
    }

    for (size_t i = 0U; i < pages; ++i) {
        vec[i] = 0U;
    }

    if (mincore(mapping, length, vec) != 0) {
        perror("mincore after");
        return 1;
    }

    printf("resident_after=%zu\n", count_resident(vec, pages));
    printf("minor_fault_delta=%ld\n", after.ru_minflt - before.ru_minflt);
    printf("pages_touched=%zu\n", pages);

    free(vec);
    (void)munmap(mapping, length);
    return 0;
}
