#include <stdio.h>

#include "math_ops.h"

#define FIRST_VALUE 7
#define SECOND_VALUE 5

int main(void)
{
    int sum = add(FIRST_VALUE, SECOND_VALUE);
    int product = multiply(FIRST_VALUE, SECOND_VALUE);

    printf("sum: %d\n", sum);
    printf("product: %d\n", product);

    return 0;
}
