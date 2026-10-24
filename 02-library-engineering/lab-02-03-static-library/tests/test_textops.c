#include "textops.h"

#include <assert.h>
#include <string.h>

int main(void)
{
    char text[] = "alpha beta  gamma";

    assert(textops_count_words("") == 0U);
    assert(textops_count_words("one") == 1U);
    assert(textops_count_words("one two three") == 3U);
    assert(textops_count_words(NULL) == 0U);

    textops_ascii_upper(text);
    assert(strcmp(text, "ALPHA BETA  GAMMA") == 0);

    textops_ascii_upper(NULL);
    return 0;
}
