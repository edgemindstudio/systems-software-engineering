#include "textops.h"

#include <stdio.h>

int main(void)
{
    char text[] = "systems libraries are reusable";

    printf("words=%zu\n", textops_count_words(text));
    textops_ascii_upper(text);
    printf("upper=%s\n", text);

    return 0;
}
