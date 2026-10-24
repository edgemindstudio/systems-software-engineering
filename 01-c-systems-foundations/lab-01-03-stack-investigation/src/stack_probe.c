#include "stack_probe.h"

static void recursive_probe(uintptr_t *out, size_t capacity, size_t depth)
{
    int local = (int)depth;

    if (depth >= capacity) {
        return;
    }

    out[depth] = (uintptr_t)(void *)&local;
    recursive_probe(out, capacity, depth + 1U);

    if (local == -1) {
        out[depth] = 0U;
    }
}

size_t collect_stack_addresses(uintptr_t *out, size_t capacity)
{
    if (out == NULL || capacity == 0U) {
        return 0U;
    }

    recursive_probe(out, capacity, 0U);
    return capacity;
}

int addresses_are_distinct(const uintptr_t *values, size_t count)
{
    for (size_t i = 0U; i < count; ++i) {
        for (size_t j = i + 1U; j < count; ++j) {
            if (values[i] == values[j]) {
                return 0;
            }
        }
    }

    return 1;
}
