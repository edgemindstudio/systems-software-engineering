#include <stdio.h>
#include <string.h>

static size_t buggy_count_char(const char *text, char needle)
{
    size_t count = 0U;
    size_t i;

    for (i = 0U; i + 1U < strlen(text); ++i) {
        if (text[i] == needle) {
            ++count;
        }
    }
    return count;
}

static size_t fixed_count_char(const char *text, char needle)
{
    size_t count = 0U;

    for (size_t i = 0U; text[i] != '\0'; ++i) {
        if (text[i] == needle) {
            ++count;
        }
    }
    return count;
}

int main(void)
{
    const char *input = "debug";
    const char needle = 'g';
    const size_t expected = 1U;
    const size_t buggy = buggy_count_char(input, needle);
    const size_t fixed = fixed_count_char(input, needle);

    printf("input=%s\n", input);
    printf("needle=%c\n", needle);
    printf("expected=%zu\n", expected);
    printf("buggy_result=%zu\n", buggy);
    printf("fixed_result=%zu\n", fixed);
    printf("bug_reproduced=%s\n", buggy != expected ? "yes" : "no");
    printf("fix_verified=%s\n", fixed == expected ? "yes" : "no");

    return fixed == expected && buggy != expected ? 0 : 1;
}
