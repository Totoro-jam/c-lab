/*
 * test_debug.c — Tests for the fixed versions.
 *
 * These tests verify correct behavior. Running with SANITIZE=1
 * additionally verifies no memory errors occur.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buggy.h"

static int g_pass = 0;
static int g_total = 0;

#define RUN(name) do { ++g_total; name(); ++g_pass; printf("[PASS] %s\n", #name); } while (0)

static void test_fixed_strdup(void) {
    char *s = fixed_strdup("hello");
    assert(s != NULL);
    assert(strcmp(s, "hello") == 0);
    free(s); /* properly freed = no leak under ASan */

    char *empty = fixed_strdup("");
    assert(empty != NULL);
    assert(strcmp(empty, "") == 0);
    free(empty);
}

static void test_fixed_array_sum(void) {
    int arr[] = {1, 2, 3, 4, 5};
    assert(fixed_array_sum(arr, 5) == 15);
    assert(fixed_array_sum(arr, 0) == 0);
    assert(fixed_array_sum(arr, 1) == 1);
}

static void test_fixed_lifecycle(void) {
    int val = fixed_lifecycle();
    assert(val == 42);
}

static void test_fixed_safe_divide(void) {
    int result;

    assert(fixed_safe_divide(10, 2, &result) == 0);
    assert(result == 5);

    assert(fixed_safe_divide(7, 3, &result) == 0);
    assert(result == 2);

    /* Division by zero returns error */
    assert(fixed_safe_divide(10, 0, &result) == -1);

    /* INT_MIN / -1 overflow returns error */
    assert(fixed_safe_divide(-2147483647 - 1, -1, &result) == -1);
}

static void test_assert_still_works(void) {
    /*
     * In debug mode: assert is active.
     * In release (NDEBUG): assert is removed.
     * This test verifies assert is present in debug builds.
     */
#ifndef NDEBUG
    /* We're in debug mode, assert should be active */
    assert(1 == 1);
    /* assert(0) would abort -- don't do that in a test! */
#endif
}

int main(void) {
    puts("Running 09-debug-sanitize tests...");

    RUN(test_fixed_strdup);
    RUN(test_fixed_array_sum);
    RUN(test_fixed_lifecycle);
    RUN(test_fixed_safe_divide);
    RUN(test_assert_still_works);

    printf("%d/%d passed\n", g_pass, g_total);
    return g_pass == g_total ? 0 : 1;
}
