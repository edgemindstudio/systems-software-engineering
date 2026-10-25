#include <stdio.h>

int global_counter = 7;

int add_values(int a, int b)
{
    return a + b + global_counter;
}

int main(void)
{
    printf("%d\n", add_values(2, 3));
    return 0;
}
