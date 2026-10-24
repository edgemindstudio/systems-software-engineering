#ifndef INT_VECTOR_H
#define INT_VECTOR_H

#include <stddef.h>

struct int_vector {
    int *data;
    size_t size;
    size_t capacity;
};

int int_vector_init(struct int_vector *vector);
void int_vector_destroy(struct int_vector *vector);
int int_vector_push(struct int_vector *vector, int value);
int int_vector_get(const struct int_vector *vector, size_t index, int *out);
int int_vector_insert(struct int_vector *vector, size_t index, int value);
int int_vector_remove(struct int_vector *vector, size_t index, int *removed);

#endif
