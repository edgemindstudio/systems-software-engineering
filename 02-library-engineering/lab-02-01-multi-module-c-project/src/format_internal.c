#include "format_internal.h"

#include <stdio.h>

int format_summary_values(
    double average_temperature,
    unsigned int peak_rpm,
    char *buffer,
    size_t buffer_size
)
{
    int written;

    if (buffer == NULL || buffer_size == 0U) {
        return 0;
    }

    written = snprintf(
        buffer,
        buffer_size,
        "avg_temp=%.2fC peak_rpm=%u",
        average_temperature,
        peak_rpm
    );

    return written >= 0 && (size_t)written < buffer_size;
}
