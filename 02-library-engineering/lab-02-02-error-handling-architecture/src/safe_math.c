#include "safe_math.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static void set_error(
    struct safe_math_error *error,
    enum safe_math_status code,
    const char *message
)
{
    if (error == NULL) {
        return;
    }

    error->code = code;
    (void)snprintf(error->message, sizeof error->message, "%s", message);
}

const char *safe_math_status_name(enum safe_math_status status)
{
    switch (status) {
        case SAFE_MATH_OK:
            return "ok";
        case SAFE_MATH_INVALID_ARGUMENT:
            return "invalid_argument";
        case SAFE_MATH_PARSE_ERROR:
            return "parse_error";
        case SAFE_MATH_RANGE_ERROR:
            return "range_error";
        case SAFE_MATH_DIVIDE_BY_ZERO:
            return "divide_by_zero";
    }

    return "unknown";
}

enum safe_math_status safe_parse_int(
    const char *text,
    int *out,
    struct safe_math_error *error
)
{
    char *end = NULL;
    long value;

    if (text == NULL || out == NULL) {
        set_error(error, SAFE_MATH_INVALID_ARGUMENT, "text and output are required");
        return SAFE_MATH_INVALID_ARGUMENT;
    }

    errno = 0;
    value = strtol(text, &end, 10);

    if (end == text || *end != '\0') {
        set_error(error, SAFE_MATH_PARSE_ERROR, "input is not a complete integer");
        return SAFE_MATH_PARSE_ERROR;
    }

    if (errno == ERANGE || value < INT_MIN || value > INT_MAX) {
        set_error(error, SAFE_MATH_RANGE_ERROR, "integer is outside int range");
        return SAFE_MATH_RANGE_ERROR;
    }

    *out = (int)value;
    set_error(error, SAFE_MATH_OK, "ok");
    return SAFE_MATH_OK;
}

enum safe_math_status safe_divide(
    int numerator,
    int denominator,
    int *out,
    struct safe_math_error *error
)
{
    if (out == NULL) {
        set_error(error, SAFE_MATH_INVALID_ARGUMENT, "output is required");
        return SAFE_MATH_INVALID_ARGUMENT;
    }

    if (denominator == 0) {
        set_error(error, SAFE_MATH_DIVIDE_BY_ZERO, "denominator must not be zero");
        return SAFE_MATH_DIVIDE_BY_ZERO;
    }

    if (numerator == INT_MIN && denominator == -1) {
        set_error(error, SAFE_MATH_RANGE_ERROR, "division result overflows int");
        return SAFE_MATH_RANGE_ERROR;
    }

    *out = numerator / denominator;
    set_error(error, SAFE_MATH_OK, "ok");
    return SAFE_MATH_OK;
}
