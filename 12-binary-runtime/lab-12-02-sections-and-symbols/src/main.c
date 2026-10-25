#include <stdio.h>

int initialized_global = 7;
int zero_initialized_global;

int exported_add(int a, int b);

int exported_add(int a, int b)
{
    return a + b +
           initialized_global +
           zero_initialized_global;
}

int main(void)
{
    printf("exported_add=%d\n",
           exported_add(2, 3));
    return 0;
}
