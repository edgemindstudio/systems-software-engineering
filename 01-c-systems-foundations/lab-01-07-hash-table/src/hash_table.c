#include "hash_table.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t hash_string(const char *text)
{
    uint64_t hash = UINT64_C(1469598103934665603);

    while (*text != '\0') {
        hash ^= (unsigned char)*text;
        hash *= UINT64_C(1099511628211);
        ++text;
    }

    return hash;
}

static char *copy_string(const char *text)
{
    const size_t length = strlen(text);
    char *copy = malloc(length + 1U);

    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, text, length + 1U);
    return copy;
}

int hash_table_init(struct hash_table *table, size_t initial_buckets)
{
    if (table == NULL || initial_buckets == 0U) {
        return 0;
    }

    table->buckets = calloc(initial_buckets, sizeof *table->buckets);

    if (table->buckets == NULL) {
        return 0;
    }

    table->bucket_count = initial_buckets;
    table->size = 0U;
    return 1;
}

static int resize_table(struct hash_table *table, size_t new_bucket_count)
{
    struct hash_entry **new_buckets;

    new_buckets = calloc(new_bucket_count, sizeof *new_buckets);

    if (new_buckets == NULL) {
        return 0;
    }

    for (size_t i = 0U; i < table->bucket_count; ++i) {
        struct hash_entry *entry = table->buckets[i];

        while (entry != NULL) {
            struct hash_entry *next = entry->next;
            const size_t index =
                (size_t)(hash_string(entry->key) % new_bucket_count);

            entry->next = new_buckets[index];
            new_buckets[index] = entry;
            entry = next;
        }
    }

    free(table->buckets);
    table->buckets = new_buckets;
    table->bucket_count = new_bucket_count;
    return 1;
}

int hash_table_put(struct hash_table *table, const char *key, int value)
{
    size_t index;
    struct hash_entry *entry;

    if (table == NULL || key == NULL || table->bucket_count == 0U) {
        return 0;
    }

    if ((table->size + 1U) * 4U > table->bucket_count * 3U) {
        if (table->bucket_count > SIZE_MAX / 2U) {
            return 0;
        }

        if (resize_table(table, table->bucket_count * 2U) == 0) {
            return 0;
        }
    }

    index = (size_t)(hash_string(key) % table->bucket_count);

    for (entry = table->buckets[index]; entry != NULL; entry = entry->next) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return 1;
        }
    }

    entry = malloc(sizeof *entry);
    if (entry == NULL) {
        return 0;
    }

    entry->key = copy_string(key);
    if (entry->key == NULL) {
        free(entry);
        return 0;
    }

    entry->value = value;
    entry->next = table->buckets[index];
    table->buckets[index] = entry;
    ++table->size;
    return 1;
}

int hash_table_get(const struct hash_table *table, const char *key, int *out)
{
    size_t index;

    if (table == NULL || key == NULL || out == NULL ||
        table->bucket_count == 0U) {
        return 0;
    }

    index = (size_t)(hash_string(key) % table->bucket_count);

    for (const struct hash_entry *entry = table->buckets[index];
         entry != NULL;
         entry = entry->next) {
        if (strcmp(entry->key, key) == 0) {
            *out = entry->value;
            return 1;
        }
    }

    return 0;
}

int hash_table_remove(struct hash_table *table, const char *key)
{
    size_t index;
    struct hash_entry *previous = NULL;
    struct hash_entry *current;

    if (table == NULL || key == NULL || table->bucket_count == 0U) {
        return 0;
    }

    index = (size_t)(hash_string(key) % table->bucket_count);
    current = table->buckets[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (previous == NULL) {
                table->buckets[index] = current->next;
            } else {
                previous->next = current->next;
            }

            free(current->key);
            free(current);
            --table->size;
            return 1;
        }

        previous = current;
        current = current->next;
    }

    return 0;
}

void hash_table_destroy(struct hash_table *table)
{
    if (table == NULL) {
        return;
    }

    for (size_t i = 0U; i < table->bucket_count; ++i) {
        struct hash_entry *entry = table->buckets[i];

        while (entry != NULL) {
            struct hash_entry *next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
    }

    free(table->buckets);
    table->buckets = NULL;
    table->bucket_count = 0U;
    table->size = 0U;
}
