#include <stdio.h>

static int crash_here(int *ptr)
{
    return *ptr;
}

int main(void)
{
    int *ptr = NULL;
    printf("about_to_crash=yes\n");
    fflush(stdout);
    return crash_here(ptr);
}
