#include "telemetry.h"

#include <assert.h>
#include <string.h>

int main(void)
{
    const struct telemetry_sample samples[] = {
        {10.0, 100U},
        {20.0, 400U},
        {30.0, 250U}
    };
    char buffer[128];

    assert(telemetry_average_temperature(samples, 3U) == 20.0);
    assert(telemetry_peak_rpm(samples, 3U) == 400U);
    assert(telemetry_average_temperature(NULL, 3U) == 0.0);
    assert(telemetry_peak_rpm(NULL, 3U) == 0U);

    assert(telemetry_format_summary(samples, 3U, buffer, sizeof buffer) != 0);
    assert(strcmp(buffer, "avg_temp=20.00C peak_rpm=400") == 0);

    assert(telemetry_format_summary(samples, 3U, buffer, 4U) == 0);
    return 0;
}
