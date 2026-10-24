#include "int_buffer.h"

#include <assert.h>

int main(void)
{
    struct int_buffer buffer;

    assert(int_buffer_init(&buffer, 1U) != 0);

    for (int i = 0; i < 100; ++i) {
        assert(int_buffer_push(&buffer, i) != 0);
    }

    assert(buffer.size == 100U);
    assert(buffer.capacity >= buffer.size);

    for (size_t i = 0U; i < buffer.size; ++i) {
        assert(buffer.data[i] == (int)i);
    }

    int_buffer_destroy(&buffer);

    assert(buffer.data == NULL);
    assert(buffer.size == 0U);
    assert(buffer.capacity == 0U);

    return 0;
}
