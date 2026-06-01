/*
 * vec.h — Generic dynamic array.
 *
 * Stores void* values. Growth strategy: 2x capacity.
 */
#ifndef DS_VEC_H
#define DS_VEC_H

#include <stddef.h>

typedef struct {
    void **items;   /* array of void* pointers */
    size_t len;
    size_t cap;
} ds_vec_t;

int ds_vec_init(ds_vec_t *v);
int ds_vec_push(ds_vec_t *v, void *item);
void *ds_vec_get(const ds_vec_t *v, size_t index);
void *ds_vec_pop(ds_vec_t *v);
size_t ds_vec_len(const ds_vec_t *v);
void ds_vec_free(ds_vec_t *v);

#endif /* DS_VEC_H */
