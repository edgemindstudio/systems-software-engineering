#ifndef SAFE_MATH_H
#define SAFE_MATH_H

#include <stddef.h>

enum safe_math_status {
    SAFE_MATH_OK = 0,
    SAFE_MATH_INVALID_ARGUMENT,
    SAFE_MATH_PARSE_ERROR,
    SAFE_MATH_RANGE_ERROR,
    SAFE_MATH_DIVIDE_BY_ZERO
};

struct safe_math_error {
    enum safe_math_status code;
    char message[128];
};

const char *safe_math_status_name(enum safe_math_status status);

enum safe_math_status safe_parse_int(
    const char *text,
    int *out,
    struct safe_math_error *error
);

enum safe_math_status safe_divide(
    int numerator,
    int denominator,
    int *out,
    struct safe_math_error *error
);

#endif
