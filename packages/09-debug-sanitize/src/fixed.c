/*
 * fixed.c — Correct versions of the common bug patterns.
 *
 * Each function here has a corresponding "buggy" version
 * documented in comments showing what the bug would look like.
 */
#include <stdlib.h>
#include <string.h>

#include "buggy.h"

/*
 * Bug #1: Memory leak
 *
 * BUGGY VERSION (leak):
 *   char *buggy_strdup(const char *s) {
 *       char *p = malloc(strlen(s) + 1);
 *       strcpy(p, s);
 *       return p;
 *       // caller forgets to free -> leak
 *   }
 *
 * FIXED: same code, but caller correctly frees.
 * The real fix is documenting ownership.
 */
char *fixed_strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *p = malloc(len);
    if (p == NULL) {
        return NULL;
    }
    memcpy(p, s, len);
    return p;
}

/*
 * Bug #2: Heap buffer overflow
 *
 * BUGGY VERSION:
 *   int buggy_array_sum(const int *arr, size_t len) {
 *       int sum = 0;
 *       for (size_t i = 0; i <= len; i++) {  // off-by-one: <= instead of <
 *           sum += arr[i];
 *       }
 *       return sum;
 *   }
 *
 * FIXED: correct bounds check.
 */
int fixed_array_sum(const int *arr, size_t len) {
    int sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum += arr[i];
    }
    return sum;
}

/*
 * Bug #3: Use-after-free
 *
 * BUGGY VERSION:
 *   int buggy_lifecycle(void) {
 *       int *p = malloc(sizeof(int));
 *       *p = 42;
 *       free(p);
 *       return *p;  // use-after-free!
 *   }
 *
 * FIXED: read before free.
 */
int fixed_lifecycle(void) {
    int *p = malloc(sizeof(int));
    if (p == NULL) {
        return -1;
    }
    *p = 42;
    int value = *p;  /* read BEFORE free */
    free(p);
    return value;
}

/*
 * Bug #4: Signed integer overflow / division by zero (UB)
 *
 * BUGGY VERSION:
 *   int buggy_divide(int a, int b) {
 *       return a / b;  // UB if b == 0 or (a == INT_MIN && b == -1)
 *   }
 *
 * FIXED: check before dividing.
 */
int fixed_safe_divide(int a, int b, int *result) {
    if (b == 0) {
        return -1; /* error: division by zero */
    }
    /* INT_MIN / -1 is undefined on two's complement */
    if (a == -2147483647 - 1 && b == -1) {
        return -1; /* error: would overflow */
    }
    *result = a / b;
    return 0;
}
