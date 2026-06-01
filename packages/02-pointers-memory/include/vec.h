/*
 * vec.h — Simple dynamic array (vector) using void*.
 *
 * Demonstrates: malloc/realloc/free, pointer arithmetic with void*,
 * grow strategy (2x capacity).
 */
#ifndef VEC_H
#define VEC_H

#include <stddef.h>

typedef struct {
    void *data;       /* heap-allocated buffer */
    size_t len;       /* number of elements currently stored */
    size_t cap;       /* allocated capacity (in elements) */
    size_t elem_size; /* sizeof each element */
} vec_t;

/*
 * Initialize a vector for elements of given size.
 * Returns 0 on success, -1 on allocation failure.
 */
int vec_init(vec_t *v, size_t elem_size);

/*
 * Push a copy of *elem onto the end of the vector.
 * Returns 0 on success, -1 on allocation failure.
 */
int vec_push(vec_t *v, const void *elem);

/*
 * Get pointer to element at index. Returns NULL if out of bounds.
 */
void *vec_get(const vec_t *v, size_t index);

/*
 * Return current length.
 */
size_t vec_len(const vec_t *v);

/*
 * Free all memory. Vector is zeroed after this call.
 */
void vec_free(vec_t *v);

#endif /* VEC_H */
