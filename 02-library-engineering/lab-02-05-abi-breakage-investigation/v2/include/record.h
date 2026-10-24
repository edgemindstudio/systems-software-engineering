#ifndef RECORD_H
#define RECORD_H

#include <stddef.h>
#include <stdint.h>

struct record {
    uint32_t id;
    uint64_t timestamp_ns;
    uint32_t value;
};

size_t record_abi_size(void);
uint64_t record_score(const struct record *record);

#endif
