/*
 * sort.h — Generic sort with function pointer comparator.
 */
#ifndef SORT_H
#define SORT_H

#include <stddef.h>

/*
 * Comparator function type.
 * Returns negative if a < b, 0 if equal, positive if a > b.
 */
typedef int (*cmp_fn)(const void *a, const void *b);

/*
 * Sort an array in place using bubble sort (simple, demonstrative).
 * base: pointer to first element
 * count: number of elements
 * size: sizeof each element
 * cmp: comparator function pointer
 */
void bubble_sort(void *base, size_t count, size_t size, cmp_fn cmp);

#endif /* SORT_H */
