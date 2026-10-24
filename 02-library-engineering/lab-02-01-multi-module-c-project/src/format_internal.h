#ifndef FORMAT_INTERNAL_H
#define FORMAT_INTERNAL_H

#include <stddef.h>

int format_summary_values(
    double average_temperature,
    unsigned int peak_rpm,
    char *buffer,
    size_t buffer_size
);

#endif
