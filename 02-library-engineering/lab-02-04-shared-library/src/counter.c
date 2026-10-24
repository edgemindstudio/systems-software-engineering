#include "counter.h"

size_t counter_count_char(const char *text, char target)
{
    size_t count = 0U;

    if (text == NULL) {
        return 0U;
    }

    while (*text != '\0') {
        if (*text == target) {
            ++count;
        }
        ++text;
    }

    return count;
}

size_t counter_count_lines(const char *text)
{
    size_t count = 0U;
    int saw_data = 0;

    if (text == NULL || *text == '\0') {
        return 0U;
    }

    while (*text != '\0') {
        saw_data = 1;
        if (*text == '\n') {
            ++count;
        }
        ++text;
    }

    if (saw_data != 0 && text[-1] != '\n') {
        ++count;
    }

    return count;
}

const char *counter_version(void)
{
    return "1.0.0";
}
