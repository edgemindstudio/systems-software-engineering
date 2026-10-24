#include "hash_table.h"

#include <assert.h>
#include <stdio.h>

int main(void)
{
    struct hash_table table;
    int value = 0;
    char key[32];

    assert(hash_table_init(&table, 4U) != 0);

    for (int i = 0; i < 100; ++i) {
        const int written = snprintf(key, sizeof key, "key-%d", i);
        assert(written > 0);
        assert((size_t)written < sizeof key);
        assert(hash_table_put(&table, key, i * 10) != 0);
    }

    assert(table.size == 100U);
    assert(table.bucket_count > 4U);

    for (int i = 0; i < 100; ++i) {
        const int written = snprintf(key, sizeof key, "key-%d", i);
        assert(written > 0);
        assert((size_t)written < sizeof key);
        assert(hash_table_get(&table, key, &value) != 0);
        assert(value == i * 10);
    }

    assert(hash_table_put(&table, "key-50", 9999) != 0);
    assert(table.size == 100U);
    assert(hash_table_get(&table, "key-50", &value) != 0);
    assert(value == 9999);

    assert(hash_table_remove(&table, "key-50") != 0);
    assert(table.size == 99U);
    assert(hash_table_get(&table, "key-50", &value) == 0);
    assert(hash_table_remove(&table, "missing") == 0);

    hash_table_destroy(&table);
    assert(table.buckets == NULL);
    assert(table.size == 0U);
    assert(table.bucket_count == 0U);

    return 0;
}
