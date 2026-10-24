#include "stack_probe.h"

#include <inttypes.h>
#include <stdio.h>

int main(void)
{
    uintptr_t addresses[6];
    const size_t count = collect_stack_addresses(addresses, 6U);

    for (size_t i = 0U; i < count; ++i) {
        printf("depth %zu address 0x%" PRIxPTR "\n", i, addresses[i]);
    }

    printf("distinct active-frame locals: %s\n",
           addresses_are_distinct(addresses, count) != 0 ? "yes" : "no");

    if (count >= 2U) {
        printf("observed address trend: %s\n",
               addresses[1] < addresses[0] ? "decreasing" : "increasing");
    }

    return 0;
}
