#include "pointer_ops.h"

#include <stdio.h>

int main(void)
{
    int values[] = {2, 4, 6, 8, 10};
    const size_t count = sizeof values / sizeof values[0];
    int *found = NULL;

    printf("sum: %ld\n", sum_array(values, count));

    if (find_first(values, count, 6, &found) != 0) {
        printf("found: %d at index %td\n", *found, found - values);
        *found = 7;
    }

    reverse_array(values, count);

    printf("reversed:");
    for (size_t i = 0U; i < count; ++i) {
        printf(" %d", values[i]);
    }
    printf("\n");

    return 0;
}
