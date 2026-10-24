#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int global_value = 7;

static void marker_function(void)
{
}

int main(void)
{
    int stack_value = 11;
    int *heap_value = malloc(sizeof *heap_value);

    if (heap_value == NULL) {
        perror("malloc");
        return 1;
    }

    *heap_value = 13;

    printf("pid=%ld\n", (long)getpid());
    printf("code=%p\n", (void *)&marker_function);
    printf("global=%p\n", (void *)&global_value);
    printf("heap=%p\n", (void *)heap_value);
    printf("stack=%p\n", (void *)&stack_value);
    printf("values=%d,%d,%d\n", global_value, *heap_value, stack_value);

    free(heap_value);
    return 0;
}
