#include "telemetry.h"

#include <stdio.h>

int main(void)
{
    const struct telemetry_sample samples[] = {
        {72.5, 1800U},
        {74.0, 2200U},
        {73.5, 2050U}
    };
    char summary[128];

    if (telemetry_format_summary(
            samples,
            sizeof samples / sizeof samples[0],
            summary,
            sizeof summary
        ) == 0) {
        fprintf(stderr, "failed to format summary\n");
        return 1;
    }

    printf("%s\n", summary);
    return 0;
}
