#include "safe_math.h"

#include <stdio.h>

int main(void)
{
    struct safe_math_error error;
    int value = 0;
    int result = 0;

    if (safe_parse_int("84", &value, &error) != SAFE_MATH_OK) {
        fprintf(stderr, "%s: %s\n", safe_math_status_name(error.code), error.message);
        return 1;
    }

    if (safe_divide(value, 2, &result, &error) != SAFE_MATH_OK) {
        fprintf(stderr, "%s: %s\n", safe_math_status_name(error.code), error.message);
        return 1;
    }

    printf("84 / 2 = %d\n", result);

    if (safe_divide(10, 0, &result, &error) != SAFE_MATH_OK) {
        printf("expected error: %s: %s\n",
               safe_math_status_name(error.code),
               error.message);
    }

    return 0;
}
