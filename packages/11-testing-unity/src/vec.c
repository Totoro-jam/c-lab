/*
 * vec.c — Dynamic array (testing subject).
 */
#include <stdlib.h>
#include "vec.h"

#define INITIAL_CAP 8

int tvec_init(tvec_t *v) {
    v->items = malloc(INITIAL_CAP * sizeof(void *));
    if (v->items == NULL) return -1;
    v->len = 0;
    v->cap = INITIAL_CAP;
    return 0;
}

int tvec_push(tvec_t *v, void *item) {
    if (v->len == v->cap) {
        size_t new_cap = v->cap * 2;
        void **new_items = realloc(v->items, new_cap * sizeof(void *));
        if (new_items == NULL) return -1;
        v->items = new_items;
        v->cap = new_cap;
    }
    v->items[v->len] = item;
    v->len++;
    return 0;
}

void *tvec_get(const tvec_t *v, size_t index) {
    if (index >= v->len) return NULL;
    return v->items[index];
}

void *tvec_pop(tvec_t *v) {
    if (v->len == 0) return NULL;
    v->len--;
    return v->items[v->len];
}

size_t tvec_len(const tvec_t *v) {
    return v->len;
}

void tvec_free(tvec_t *v) {
    free(v->items);
    v->items = NULL;
    v->len = 0;
    v->cap = 0;
}
