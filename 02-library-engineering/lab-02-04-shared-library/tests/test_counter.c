#include "counter.h"

#include <assert.h>
#include <string.h>

int main(void)
{
    assert(strcmp(counter_version(), "1.0.0") == 0);
    assert(counter_count_char("banana", 'a') == 3U);
    assert(counter_count_char(NULL, 'a') == 0U);
    assert(counter_count_lines("") == 0U);
    assert(counter_count_lines("one") == 1U);
    assert(counter_count_lines("one\n") == 1U);
    assert(counter_count_lines("one\ntwo") == 2U);
    return 0;
}
