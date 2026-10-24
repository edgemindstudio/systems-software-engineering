#include "memory_representation.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

int main(void)
{
    unsigned char bytes[sizeof(uint32_t)];
    const uint32_t value = UINT32_C(0x01020304);
    enum byte_order order;

    object_bytes_u32(value, bytes);
    order = detect_byte_order();

    assert(sizeof(uint32_t) == 4U);
    assert(offsetof(struct layout_probe, tag) == 0U);
    assert(offsetof(struct layout_probe, value) >= sizeof(char));
    assert(sizeof(struct layout_probe) >=
           sizeof(char) + sizeof(uint32_t) + sizeof(uint16_t));

    if (order == BYTE_ORDER_LITTLE) {
        assert(bytes[0] == 0x04U);
        assert(bytes[3] == 0x01U);
    } else if (order == BYTE_ORDER_BIG) {
        assert(bytes[0] == 0x01U);
        assert(bytes[3] == 0x04U);
    }

    return 0;
}
