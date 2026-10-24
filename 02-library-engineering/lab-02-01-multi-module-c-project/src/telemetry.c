#include "telemetry.h"

#include "format_internal.h"

double telemetry_average_temperature(
    const struct telemetry_sample *samples,
    size_t count
)
{
    double total = 0.0;

    if (samples == NULL || count == 0U) {
        return 0.0;
    }

    for (size_t i = 0U; i < count; ++i) {
        total += samples[i].temperature_c;
    }

    return total / (double)count;
}

unsigned int telemetry_peak_rpm(
    const struct telemetry_sample *samples,
    size_t count
)
{
    unsigned int peak = 0U;

    if (samples == NULL) {
        return 0U;
    }

    for (size_t i = 0U; i < count; ++i) {
        if (samples[i].rpm > peak) {
            peak = samples[i].rpm;
        }
    }

    return peak;
}

int telemetry_format_summary(
    const struct telemetry_sample *samples,
    size_t count,
    char *buffer,
    size_t buffer_size
)
{
    if (samples == NULL || count == 0U) {
        return 0;
    }

    return format_summary_values(
        telemetry_average_temperature(samples, count),
        telemetry_peak_rpm(samples, count),
        buffer,
        buffer_size
    );
}
