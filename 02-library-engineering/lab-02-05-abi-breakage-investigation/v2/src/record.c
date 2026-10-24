#include "record.h"

size_t record_abi_size(void)
{
    return sizeof(struct record);
}

uint64_t record_score(const struct record *record)
{
    if (record == NULL) {
        return 0U;
    }

    return (uint64_t)record->id * UINT64_C(1000) +
           (uint64_t)record->value;
}
