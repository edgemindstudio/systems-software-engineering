#include "pointer_ops.h"

#include <assert.h>
#include <stddef.h>

int main(void)
{
    int values[] = {1, 2, 3, 4};
    int *found = NULL;

    assert(sum_array(values, 4U) == 10L);
    assert(find_first(values, 4U, 3, &found) == 1);
    assert(found == &values[2]);

    swap_ints(&values[0], &values[3]);
    assert(values[0] == 4);
    assert(values[3] == 1);

    reverse_array(values, 4U);
    assert(values[0] == 1);
    assert(values[1] == 3);
    assert(values[2] == 2);
    assert(values[3] == 4);

    assert(find_first(values, 4U, 99, &found) == 0);
    assert(found == NULL);

    return 0;
}
