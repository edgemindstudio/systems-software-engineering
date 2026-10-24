#include "memory_representation.h"

#include <string.h>

enum byte_order detect_byte_order(void)
{
    const uint32_t value = UINT32_C(0x01020304);
    unsigned char bytes[sizeof value];

    memcpy(bytes, &value, sizeof value);

    if (bytes[0] == 0x04U && bytes[3] == 0x01U) {
        return BYTE_ORDER_LITTLE;
    }

    if (bytes[0] == 0x01U && bytes[3] == 0x04U) {
        return BYTE_ORDER_BIG;
    }

    return BYTE_ORDER_OTHER;
}

void object_bytes_u32(uint32_t value, unsigned char out[sizeof(uint32_t)])
{
    memcpy(out, &value, sizeof value);
}
