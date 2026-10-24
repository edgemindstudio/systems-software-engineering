#ifndef INT_BUFFER_H
#define INT_BUFFER_H

#include <stddef.h>

struct int_buffer {
    int *data;
    size_t size;
    size_t capacity;
};

int int_buffer_init(struct int_buffer *buffer, size_t initial_capacity);
int int_buffer_push(struct int_buffer *buffer, int value);
void int_buffer_destroy(struct int_buffer *buffer);

#endif
