#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>

struct hash_entry {
    char *key;
    int value;
    struct hash_entry *next;
};

struct hash_table {
    struct hash_entry **buckets;
    size_t bucket_count;
    size_t size;
};

int hash_table_init(struct hash_table *table, size_t initial_buckets);
void hash_table_destroy(struct hash_table *table);
int hash_table_put(struct hash_table *table, const char *key, int value);
int hash_table_get(const struct hash_table *table, const char *key, int *out);
int hash_table_remove(struct hash_table *table, const char *key);

#endif
