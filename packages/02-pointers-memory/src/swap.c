/*
 * swap.c — Generic swap implementation using memcpy.
 *
 * Demonstrates void* as "any type" and manual memory operations.
 */
#include <string.h>
#include <stdint.h>

#include "swap.h"

void generic_swap(void *a, void *b, size_t size) {
    unsigned char tmp[256];
    unsigned char *pa = (unsigned char *)a;
    unsigned char *pb = (unsigned char *)b;

    while (size > 0) {
        size_t chunk = size > sizeof(tmp) ? sizeof(tmp) : size;
        memcpy(tmp, pa, chunk);
        memcpy(pa, pb, chunk);
        memcpy(pb, tmp, chunk);
        pa += chunk;
        pb += chunk;
        size -= chunk;
    }
}
