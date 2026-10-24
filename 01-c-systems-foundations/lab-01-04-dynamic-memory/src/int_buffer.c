#include "int_buffer.h"

#include <stdint.h>
#include <stdlib.h>

int int_buffer_init(struct int_buffer *buffer, size_t initial_capacity)
{
    if (buffer == NULL) {
        return 0;
    }

    buffer->data = NULL;
    buffer->size = 0U;
    buffer->capacity = 0U;

    if (initial_capacity == 0U) {
        return 1;
    }

    if (initial_capacity > SIZE_MAX / sizeof *buffer->data) {
        return 0;
    }

    buffer->data = malloc(initial_capacity * sizeof *buffer->data);

    if (buffer->data == NULL) {
        return 0;
    }

    buffer->capacity = initial_capacity;
    return 1;
}

static int int_buffer_grow(struct int_buffer *buffer)
{
    size_t new_capacity;
    int *new_data;

    if (buffer->capacity == 0U) {
        new_capacity = 4U;
    } else {
        if (buffer->capacity > SIZE_MAX / 2U) {
            return 0;
        }
        new_capacity = buffer->capacity * 2U;
    }

    if (new_capacity > SIZE_MAX / sizeof *buffer->data) {
        return 0;
    }

    new_data = realloc(buffer->data, new_capacity * sizeof *buffer->data);

    if (new_data == NULL) {
        return 0;
    }

    buffer->data = new_data;
    buffer->capacity = new_capacity;
    return 1;
}

int int_buffer_push(struct int_buffer *buffer, int value)
{
    if (buffer == NULL) {
        return 0;
    }

    if (buffer->size == buffer->capacity && int_buffer_grow(buffer) == 0) {
        return 0;
    }

    buffer->data[buffer->size] = value;
    ++buffer->size;
    return 1;
}

void int_buffer_destroy(struct int_buffer *buffer)
{
    if (buffer == NULL) {
        return;
    }

    free(buffer->data);
    buffer->data = NULL;
    buffer->size = 0U;
    buffer->capacity = 0U;
}
