/*
 * vec.h — Dynamic array (copied from ch06 for testing).
 */
#ifndef TEST_VEC_H
#define TEST_VEC_H

#include <stddef.h>

typedef struct {
    void **items;
    size_t len;
    size_t cap;
} tvec_t;

int tvec_init(tvec_t *v);
int tvec_push(tvec_t *v, void *item);
void *tvec_get(const tvec_t *v, size_t index);
void *tvec_pop(tvec_t *v);
size_t tvec_len(const tvec_t *v);
void tvec_free(tvec_t *v);

#endif /* TEST_VEC_H */
