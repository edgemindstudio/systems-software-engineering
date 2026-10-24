#include "counter.h"

#include <stdio.h>

int main(void)
{
    const char *text = "alpha\nbeta\ngamma";

    printf("library_version=%s\n", counter_version());
    printf("lines=%zu\n", counter_count_lines(text));
    printf("letter_a=%zu\n", counter_count_char(text, 'a'));

    return 0;
}
