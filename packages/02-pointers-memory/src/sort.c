/*
 * sort.c — Generic bubble sort with function pointer comparator.
 *
 * Demonstrates:
 *  - Function pointers as parameters
 *  - Pointer arithmetic on void* (cast to char* for byte offsets)
 *  - Generic programming in C via void*
 */
#include <stdint.h>

#include "sort.h"
#include "swap.h"

void bubble_sort(void *base, size_t count, size_t size, cmp_fn cmp) {
    if (count <= 1) {
        return;
    }

    unsigned char *arr = (unsigned char *)base;

    for (size_t i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (size_t j = 0; j < count - 1 - i; j++) {
            void *a = arr + j * size;
            void *b = arr + (j + 1) * size;
            if (cmp(a, b) > 0) {
                generic_swap(a, b, size);
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }
}
