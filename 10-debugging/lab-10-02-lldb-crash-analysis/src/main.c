#include <stdio.h>

static void trigger_fault(void)
{
    volatile int *ptr = NULL;
    *ptr = 42;
}

int main(void)
{
    puts("lldb_fault_expected=yes");
    fflush(stdout);
    trigger_fault();
    return 0;
}
