/*
 * vec.c — Dynamic array storing void* pointers.
 */
#include <stdlib.h>
#include "vec.h"

#define INITIAL_CAP 8

int ds_vec_init(ds_vec_t *v) {
    v->items = malloc(INITIAL_CAP * sizeof(void *));
    if (v->items == NULL) {
        return -1;
    }
    v->len = 0;
    v->cap = INITIAL_CAP;
    return 0;
}

int ds_vec_push(ds_vec_t *v, void *item) {
    if (v->len == v->cap) {
        size_t new_cap = v->cap * 2;
        void **new_items = realloc(v->items, new_cap * sizeof(void *));
        if (new_items == NULL) {
            return -1;
        }
        v->items = new_items;
        v->cap = new_cap;
    }
    v->items[v->len] = item;
    v->len++;
    return 0;
}

void *ds_vec_get(const ds_vec_t *v, size_t index) {
    if (index >= v->len) {
        return NULL;
    }
    return v->items[index];
}

void *ds_vec_pop(ds_vec_t *v) {
    if (v->len == 0) {
        return NULL;
    }
    v->len--;
    return v->items[v->len];
}

size_t ds_vec_len(const ds_vec_t *v) {
    return v->len;
}

void ds_vec_free(ds_vec_t *v) {
    free(v->items);
    v->items = NULL;
    v->len = 0;
    v->cap = 0;
}
