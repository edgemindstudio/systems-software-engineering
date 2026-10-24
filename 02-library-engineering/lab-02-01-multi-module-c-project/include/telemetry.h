#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <stddef.h>

struct telemetry_sample {
    double temperature_c;
    unsigned int rpm;
};

double telemetry_average_temperature(
    const struct telemetry_sample *samples,
    size_t count
);

unsigned int telemetry_peak_rpm(
    const struct telemetry_sample *samples,
    size_t count
);

int telemetry_format_summary(
    const struct telemetry_sample *samples,
    size_t count,
    char *buffer,
    size_t buffer_size
);

#endif
