#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *value = malloc(sizeof *value);
    if (value == NULL) {
        return 1;
    }

    *value = 123;
    free(value);

    printf("post_free_value=%d\n", *value);
    return 0;
}
