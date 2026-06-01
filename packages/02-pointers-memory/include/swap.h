/*
 * swap.h — Generic swap using void* and memcpy.
 */
#ifndef SWAP_H
#define SWAP_H

#include <stddef.h>

/*
 * Swap two elements of arbitrary type.
 * a, b: pointers to elements
 * size: sizeof each element
 */
void generic_swap(void *a, void *b, size_t size);

#endif /* SWAP_H */
