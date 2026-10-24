#include "int_buffer.h"

#include <stdio.h>

int main(void)
{
    struct int_buffer buffer;

    if (int_buffer_init(&buffer, 2U) == 0) {
        fprintf(stderr, "allocation failed\n");
        return 1;
    }

    for (int i = 1; i <= 10; ++i) {
        if (int_buffer_push(&buffer, i * 10) == 0) {
            fprintf(stderr, "growth failed\n");
            int_buffer_destroy(&buffer);
            return 1;
        }
    }

    printf("size=%zu capacity=%zu values:", buffer.size, buffer.capacity);
    for (size_t i = 0U; i < buffer.size; ++i) {
        printf(" %d", buffer.data[i]);
    }
    printf("\n");

    int_buffer_destroy(&buffer);
    return 0;
}
