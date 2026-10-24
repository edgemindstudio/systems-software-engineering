#ifndef STACK_PROBE_H
#define STACK_PROBE_H

#include <stddef.h>
#include <stdint.h>

size_t collect_stack_addresses(uintptr_t *out, size_t capacity);
int addresses_are_distinct(const uintptr_t *values, size_t count);

#endif
