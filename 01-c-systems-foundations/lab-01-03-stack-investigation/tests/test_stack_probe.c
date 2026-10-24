#include "stack_probe.h"

#include <assert.h>
#include <stdint.h>

int main(void)
{
    uintptr_t addresses[5];
    const size_t count = collect_stack_addresses(addresses, 5U);

    assert(count == 5U);
    assert(addresses_are_distinct(addresses, count) != 0);

    for (size_t i = 0U; i < count; ++i) {
        assert(addresses[i] != (uintptr_t)0U);
    }

    return 0;
}
