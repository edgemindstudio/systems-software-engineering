#include "int_vector.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int ensure_capacity(struct int_vector *vector, size_t required)
{
    size_t new_capacity;
    int *new_data;

    if (required <= vector->capacity) {
        return 1;
    }

    new_capacity = vector->capacity == 0U ? 4U : vector->capacity;

    while (new_capacity < required) {
        if (new_capacity > SIZE_MAX / 2U) {
            return 0;
        }
        new_capacity *= 2U;
    }

    if (new_capacity > SIZE_MAX / sizeof *vector->data) {
        return 0;
    }

    new_data = realloc(vector->data, new_capacity * sizeof *vector->data);

    if (new_data == NULL) {
        return 0;
    }

    vector->data = new_data;
    vector->capacity = new_capacity;
    return 1;
}

int int_vector_init(struct int_vector *vector)
{
    if (vector == NULL) {
        return 0;
    }

    vector->data = NULL;
    vector->size = 0U;
    vector->capacity = 0U;
    return 1;
}

void int_vector_destroy(struct int_vector *vector)
{
    if (vector == NULL) {
        return;
    }

    free(vector->data);
    vector->data = NULL;
    vector->size = 0U;
    vector->capacity = 0U;
}

int int_vector_push(struct int_vector *vector, int value)
{
    return int_vector_insert(vector, vector != NULL ? vector->size : 0U, value);
}

int int_vector_get(const struct int_vector *vector, size_t index, int *out)
{
    if (vector == NULL || out == NULL || index >= vector->size) {
        return 0;
    }

    *out = vector->data[index];
    return 1;
}

int int_vector_insert(struct int_vector *vector, size_t index, int value)
{
    size_t move_count;

    if (vector == NULL || index > vector->size) {
        return 0;
    }

    if (ensure_capacity(vector, vector->size + 1U) == 0) {
        return 0;
    }

    move_count = vector->size - index;

    if (move_count > 0U) {
        memmove(&vector->data[index + 1U],
                &vector->data[index],
                move_count * sizeof *vector->data);
    }

    vector->data[index] = value;
    ++vector->size;
    return 1;
}

int int_vector_remove(struct int_vector *vector, size_t index, int *removed)
{
    size_t move_count;

    if (vector == NULL || index >= vector->size) {
        return 0;
    }

    if (removed != NULL) {
        *removed = vector->data[index];
    }

    move_count = vector->size - index - 1U;

    if (move_count > 0U) {
        memmove(&vector->data[index],
                &vector->data[index + 1U],
                move_count * sizeof *vector->data);
    }

    --vector->size;
    return 1;
}
