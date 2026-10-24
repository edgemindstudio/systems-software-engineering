#include "memory_representation.h"

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

static const char *byte_order_name(enum byte_order order)
{
    switch (order) {
        case BYTE_ORDER_LITTLE:
            return "little-endian";
        case BYTE_ORDER_BIG:
            return "big-endian";
        case BYTE_ORDER_OTHER:
            return "other";
    }

    return "unknown";
}

int main(void)
{
    const uint32_t value = UINT32_C(0x01020304);
    unsigned char bytes[sizeof value];

    object_bytes_u32(value, bytes);

    printf("value: 0x%08" PRIx32 "\n", value);
    printf("bytes:");

    for (size_t i = 0U; i < sizeof bytes; ++i) {
        printf(" %02x", (unsigned int)bytes[i]);
    }

    printf("\n");
    printf("byte order: %s\n", byte_order_name(detect_byte_order()));
    printf("sizeof(uint32_t): %zu\n", sizeof(uint32_t));
    printf("sizeof(layout_probe): %zu\n", sizeof(struct layout_probe));
    printf("alignof(layout_probe): %zu\n", _Alignof(struct layout_probe));
    printf("offset(tag): %zu\n", offsetof(struct layout_probe, tag));
    printf("offset(value): %zu\n", offsetof(struct layout_probe, value));
    printf("offset(code): %zu\n", offsetof(struct layout_probe, code));

    return 0;
}
