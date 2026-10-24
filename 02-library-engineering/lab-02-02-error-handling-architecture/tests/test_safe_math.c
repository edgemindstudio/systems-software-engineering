#include "safe_math.h"

#include <assert.h>
#include <limits.h>
#include <string.h>

int main(void)
{
    struct safe_math_error error;
    int value = 0;

    assert(safe_parse_int("123", &value, &error) == SAFE_MATH_OK);
    assert(value == 123);

    assert(safe_parse_int("12x", &value, &error) == SAFE_MATH_PARSE_ERROR);
    assert(error.code == SAFE_MATH_PARSE_ERROR);

    assert(safe_parse_int(NULL, &value, &error) == SAFE_MATH_INVALID_ARGUMENT);

    assert(safe_divide(20, 4, &value, &error) == SAFE_MATH_OK);
    assert(value == 5);

    assert(safe_divide(20, 0, &value, &error) == SAFE_MATH_DIVIDE_BY_ZERO);
    assert(strstr(error.message, "zero") != NULL);

    assert(safe_divide(INT_MIN, -1, &value, &error) == SAFE_MATH_RANGE_ERROR);
    assert(safe_divide(1, 1, NULL, &error) == SAFE_MATH_INVALID_ARGUMENT);

    return 0;
}
