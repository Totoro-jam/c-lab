/*
 * buggy.h — Interface for demonstrating common C bugs.
 *
 * "fixed" versions are the correct implementations.
 * "buggy" versions in src/buggy.c are intentionally broken
 * (meant to be compiled with sanitizers to see error reports).
 */
#ifndef BUGGY_H
#define BUGGY_H

#include <stddef.h>

/* --- Fixed versions (tested) --- */

/*
 * Safely allocate and return a string copy.
 * Caller must free the result.
 */
char *fixed_strdup(const char *s);

/*
 * Sum array elements safely (bounds-checked).
 */
int fixed_array_sum(const int *arr, size_t len);

/*
 * Allocate, use, and properly free memory.
 */
int fixed_lifecycle(void);

/*
 * Integer operation that avoids undefined behavior.
 */
int fixed_safe_divide(int a, int b, int *result);

#endif /* BUGGY_H */
