/*
 * hashmap.c — Hash map with separate chaining.
 *
 * Hash function: FNV-1a (simple, good distribution for strings).
 */
#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

/* FNV-1a hash */
static size_t hash_string(const char *key, size_t num_buckets) {
    size_t hash = 14695981039346656037ULL;
    for (const char *p = key; *p; p++) {
        hash ^= (unsigned char)*p;
        hash *= 1099511628211ULL;
    }
    return hash % num_buckets;
}

int ds_hashmap_init(ds_hashmap_t *map, size_t initial_buckets) {
    if (initial_buckets == 0) {
        initial_buckets = 16;
    }
    map->buckets = calloc(initial_buckets, sizeof(ds_hm_entry_t *));
    if (map->buckets == NULL) {
        return -1;
    }
    map->num_buckets = initial_buckets;
    map->len = 0;
    return 0;
}

int ds_hashmap_put(ds_hashmap_t *map, const char *key, void *value) {
    size_t idx = hash_string(key, map->num_buckets);

    /* Check if key already exists */
    ds_hm_entry_t *entry = map->buckets[idx];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value; /* update existing */
            return 0;
        }
        entry = entry->next;
    }

    /* Create new entry */
    ds_hm_entry_t *new_entry = malloc(sizeof(ds_hm_entry_t));
    if (new_entry == NULL) {
        return -1;
    }
    new_entry->key = strdup(key);
    if (new_entry->key == NULL) {
        free(new_entry);
        return -1;
    }
    new_entry->value = value;
    new_entry->next = map->buckets[idx];
    map->buckets[idx] = new_entry;
    map->len++;
    return 0;
}

void *ds_hashmap_get(const ds_hashmap_t *map, const char *key) {
    size_t idx = hash_string(key, map->num_buckets);
    ds_hm_entry_t *entry = map->buckets[idx];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

int ds_hashmap_remove(ds_hashmap_t *map, const char *key) {
    size_t idx = hash_string(key, map->num_buckets);
    ds_hm_entry_t **prev = &map->buckets[idx];
    ds_hm_entry_t *entry = map->buckets[idx];

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            *prev = entry->next;
            free(entry->key);
            free(entry);
            map->len--;
            return 0;
        }
        prev = &entry->next;
        entry = entry->next;
    }
    return -1; /* key not found */
}

size_t ds_hashmap_len(const ds_hashmap_t *map) {
    return map->len;
}

void ds_hashmap_free(ds_hashmap_t *map) {
    for (size_t i = 0; i < map->num_buckets; i++) {
        ds_hm_entry_t *entry = map->buckets[i];
        while (entry != NULL) {
            ds_hm_entry_t *next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
    }
    free(map->buckets);
    map->buckets = NULL;
    map->num_buckets = 0;
    map->len = 0;
}
