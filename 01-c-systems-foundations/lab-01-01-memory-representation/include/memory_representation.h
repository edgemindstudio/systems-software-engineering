#ifndef MEMORY_REPRESENTATION_H
#define MEMORY_REPRESENTATION_H

#include <stddef.h>
#include <stdint.h>

enum byte_order {
    BYTE_ORDER_LITTLE,
    BYTE_ORDER_BIG,
    BYTE_ORDER_OTHER
};

struct layout_probe {
    char tag;
    uint32_t value;
    uint16_t code;
};

enum byte_order detect_byte_order(void);
void object_bytes_u32(uint32_t value, unsigned char out[sizeof(uint32_t)]);

#endif
