#include <stdlib.h>

int main(void)
{
    int *values = malloc(4U * sizeof *values);
    if (values == NULL) {
        return 1;
    }

    for (int i = 0; i < 4; ++i) {
        values[i] = i;
    }

    values[4] = 99;
    free(values);
    return 0;
}
