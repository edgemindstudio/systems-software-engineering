#include "int_vector.h"

#include <assert.h>

int main(void)
{
    struct int_vector vector;
    int value = 0;
    int removed = 0;

    assert(int_vector_init(&vector) != 0);

    for (int i = 0; i < 64; ++i) {
        assert(int_vector_push(&vector, i) != 0);
    }

    assert(vector.size == 64U);
    assert(vector.capacity >= 64U);
    assert(int_vector_get(&vector, 31U, &value) != 0);
    assert(value == 31);

    assert(int_vector_insert(&vector, 32U, 999) != 0);
    assert(vector.size == 65U);
    assert(int_vector_get(&vector, 32U, &value) != 0);
    assert(value == 999);

    assert(int_vector_remove(&vector, 32U, &removed) != 0);
    assert(removed == 999);
    assert(vector.size == 64U);

    assert(int_vector_get(&vector, 1000U, &value) == 0);
    assert(int_vector_insert(&vector, vector.size + 1U, 5) == 0);

    int_vector_destroy(&vector);
    return 0;
}
