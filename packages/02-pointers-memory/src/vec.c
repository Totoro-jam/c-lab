/*
 * vec.c — Dynamic array implementation.
 *
 * Demonstrates:
 *  - malloc / realloc / free lifecycle
 *  - 2x growth strategy for amortized O(1) push
 *  - Proper cleanup patterns
 */
#include <stdlib.h>
#include <string.h>

#include "vec.h"

#define VEC_INITIAL_CAP 4

int vec_init(vec_t *v, size_t elem_size) {
    v->data = malloc(elem_size * VEC_INITIAL_CAP);
    if (v->data == NULL) {
        return -1;
    }
    v->len = 0;
    v->cap = VEC_INITIAL_CAP;
    v->elem_size = elem_size;
    return 0;
}

int vec_push(vec_t *v, const void *elem) {
    if (v->len == v->cap) {
        size_t new_cap = v->cap * 2;
        void *new_data = realloc(v->data, v->elem_size * new_cap);
        if (new_data == NULL) {
            return -1;
        }
        v->data = new_data;
        v->cap = new_cap;
    }
    unsigned char *dest = (unsigned char *)v->data + v->len * v->elem_size;
    memcpy(dest, elem, v->elem_size);
    v->len++;
    return 0;
}

void *vec_get(const vec_t *v, size_t index) {
    if (index >= v->len) {
        return NULL;
    }
    return (unsigned char *)v->data + index * v->elem_size;
}

size_t vec_len(const vec_t *v) {
    return v->len;
}

void vec_free(vec_t *v) {
    free(v->data);
    v->data = NULL;
    v->len = 0;
    v->cap = 0;
    v->elem_size = 0;
}
