#include "hash_table.h"

#include <stdio.h>

int main(void)
{
    struct hash_table table;
    int value = 0;

    if (hash_table_init(&table, 4U) == 0) {
        return 1;
    }

    (void)hash_table_put(&table, "alpha", 10);
    (void)hash_table_put(&table, "beta", 20);
    (void)hash_table_put(&table, "gamma", 30);
    (void)hash_table_put(&table, "delta", 40);
    (void)hash_table_put(&table, "beta", 200);

    if (hash_table_get(&table, "beta", &value) != 0) {
        printf("beta=%d\n", value);
    }

    (void)hash_table_remove(&table, "gamma");

    printf("size=%zu buckets=%zu\n", table.size, table.bucket_count);

    hash_table_destroy(&table);
    return 0;
}
