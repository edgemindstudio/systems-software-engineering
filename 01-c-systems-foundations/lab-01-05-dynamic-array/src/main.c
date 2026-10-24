#include "int_vector.h"

#include <stdio.h>

int main(void)
{
    struct int_vector vector;
    int removed = 0;

    if (int_vector_init(&vector) == 0) {
        return 1;
    }

    for (int value = 10; value <= 50; value += 10) {
        if (int_vector_push(&vector, value) == 0) {
            int_vector_destroy(&vector);
            return 1;
        }
    }

    (void)int_vector_insert(&vector, 2U, 25);
    (void)int_vector_remove(&vector, 4U, &removed);

    printf("size=%zu capacity=%zu removed=%d values:",
           vector.size, vector.capacity, removed);

    for (size_t i = 0U; i < vector.size; ++i) {
        printf(" %d", vector.data[i]);
    }
    printf("\n");

    int_vector_destroy(&vector);
    return 0;
}
