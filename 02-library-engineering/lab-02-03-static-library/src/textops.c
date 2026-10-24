#include "textops.h"

#include <ctype.h>

size_t textops_count_words(const char *text)
{
    size_t count = 0U;
    int in_word = 0;

    if (text == NULL) {
        return 0U;
    }

    while (*text != '\0') {
        if (isspace((unsigned char)*text) != 0) {
            in_word = 0;
        } else if (in_word == 0) {
            in_word = 1;
            ++count;
        }

        ++text;
    }

    return count;
}

void textops_ascii_upper(char *text)
{
    if (text == NULL) {
        return;
    }

    while (*text != '\0') {
        if (*text >= 'a' && *text <= 'z') {
            *text = (char)(*text - ('a' - 'A'));
        }
        ++text;
    }
}
