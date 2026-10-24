#ifndef POINTER_OPS_H
#define POINTER_OPS_H

#include <stddef.h>

long sum_array(const int *values, size_t count);
void reverse_array(int *values, size_t count);
int find_first(int *values, size_t count, int target, int **out);
void swap_ints(int *left, int *right);

#endif
