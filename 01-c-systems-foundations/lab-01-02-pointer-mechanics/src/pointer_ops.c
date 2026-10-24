#include "pointer_ops.h"

long sum_array(const int *values, size_t count)
{
    long total = 0;

    for (const int *cursor = values; cursor < values + count; ++cursor) {
        total += (long)*cursor;
    }

    return total;
}

void swap_ints(int *left, int *right)
{
    const int temporary = *left;
    *left = *right;
    *right = temporary;
}

void reverse_array(int *values, size_t count)
{
    if (count < 2U) {
        return;
    }

    int *left = values;
    int *right = values + count - 1U;

    while (left < right) {
        swap_ints(left, right);
        ++left;
        --right;
    }
}

int find_first(int *values, size_t count, int target, int **out)
{
    for (size_t i = 0U; i < count; ++i) {
        if (values[i] == target) {
            *out = &values[i];
            return 1;
        }
    }

    *out = NULL;
    return 0;
}
