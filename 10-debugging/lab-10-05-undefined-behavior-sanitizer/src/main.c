#include <limits.h>
#include <stdio.h>

int main(void)
{
    volatile int value = INT_MAX;
    volatile int result = value + 1;
    printf("overflow_result=%d\n", result);
    return 0;
}
