#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *value = malloc(sizeof *value);
    if (value == NULL) return 1;

    *value = 42;
    free(value);

    printf("about_to_read_freed_memory=yes\n");
    fflush(stdout);

    return *value;
}
