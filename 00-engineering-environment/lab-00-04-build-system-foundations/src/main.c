#include <stdio.h>

#include "math_ops.h"

int main(void)
{
    const int first = 8;
    const int second = 6;

    printf("add: %d\n", add(first, second));
    printf("multiply: %d\n", multiply(first, second));

    return 0;
}
