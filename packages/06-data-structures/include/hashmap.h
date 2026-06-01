/*
 * hashmap.h — Simple hash map (string keys, void* values).
 *
 * Uses separate chaining (linked list per bucket).
 */
#ifndef DS_HASHMAP_H
#define DS_HASHMAP_H

#include <stddef.h>

typedef struct ds_hm_entry {
    char *key;                  /* heap-allocated copy of key */
    void *value;
    struct ds_hm_entry *next;  /* chain for collisions */
} ds_hm_entry_t;

typedef struct {
    ds_hm_entry_t **buckets;
    size_t num_buckets;
    size_t len;
} ds_hashmap_t;

int ds_hashmap_init(ds_hashmap_t *map, size_t initial_buckets);
int ds_hashmap_put(ds_hashmap_t *map, const char *key, void *value);
void *ds_hashmap_get(const ds_hashmap_t *map, const char *key);
int ds_hashmap_remove(ds_hashmap_t *map, const char *key);
size_t ds_hashmap_len(const ds_hashmap_t *map);
void ds_hashmap_free(ds_hashmap_t *map);

#endif /* DS_HASHMAP_H */
