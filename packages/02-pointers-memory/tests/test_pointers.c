/*
 * test_pointers.c — Tests for swap, sort, and vec.
 *
 * Assert-based, no external framework.
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "swap.h"
#include "sort.h"
#include "vec.h"

static int g_pass = 0;
static int g_total = 0;

#define RUN(name) do { ++g_total; name(); ++g_pass; printf("[PASS] %s\n", #name); } while (0)

/* --- swap tests --- */

static void test_swap_int(void) {
    int a = 42, b = 99;
    generic_swap(&a, &b, sizeof(int));
    assert(a == 99);
    assert(b == 42);
}

static void test_swap_double(void) {
    double a = 3.14, b = 2.71;
    generic_swap(&a, &b, sizeof(double));
    assert(a == 2.71);
    assert(b == 3.14);
}

static void test_swap_struct(void) {
    struct point { int x; int y; };
    struct point a = {1, 2}, b = {3, 4};
    generic_swap(&a, &b, sizeof(struct point));
    assert(a.x == 3 && a.y == 4);
    assert(b.x == 1 && b.y == 2);
}

/* --- sort tests --- */

static int cmp_int(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

static int cmp_int_desc(const void *a, const void *b) {
    return cmp_int(b, a);
}

static void test_sort_ints(void) {
    int arr[] = {5, 3, 8, 1, 9, 2};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    bubble_sort(arr, n, sizeof(int), cmp_int);
    for (size_t i = 0; i < n - 1; i++) {
        assert(arr[i] <= arr[i + 1]);
    }
}

static void test_sort_descending(void) {
    int arr[] = {5, 3, 8, 1, 9, 2};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    bubble_sort(arr, n, sizeof(int), cmp_int_desc);
    for (size_t i = 0; i < n - 1; i++) {
        assert(arr[i] >= arr[i + 1]);
    }
}

static void test_sort_already_sorted(void) {
    int arr[] = {1, 2, 3, 4, 5};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    bubble_sort(arr, n, sizeof(int), cmp_int);
    for (size_t i = 0; i < n - 1; i++) {
        assert(arr[i] <= arr[i + 1]);
    }
}

static void test_sort_single_element(void) {
    int arr[] = {42};
    bubble_sort(arr, 1, sizeof(int), cmp_int);
    assert(arr[0] == 42);
}

static void test_sort_empty(void) {
    int arr[] = {0};
    bubble_sort(arr, 0, sizeof(int), cmp_int);
    /* should not crash */
}

/* --- vec tests --- */

static void test_vec_push_and_get(void) {
    vec_t v;
    assert(vec_init(&v, sizeof(int)) == 0);

    for (int i = 0; i < 10; i++) {
        assert(vec_push(&v, &i) == 0);
    }
    assert(vec_len(&v) == 10);

    for (int i = 0; i < 10; i++) {
        int *p = (int *)vec_get(&v, (size_t)i);
        assert(p != NULL);
        assert(*p == i);
    }

    vec_free(&v);
}

static void test_vec_out_of_bounds(void) {
    vec_t v;
    assert(vec_init(&v, sizeof(int)) == 0);

    int x = 1;
    vec_push(&v, &x);
    assert(vec_get(&v, 0) != NULL);
    assert(vec_get(&v, 1) == NULL);
    assert(vec_get(&v, 100) == NULL);

    vec_free(&v);
}

static void test_vec_grow(void) {
    vec_t v;
    assert(vec_init(&v, sizeof(int)) == 0);

    /* Push enough to trigger multiple grows */
    for (int i = 0; i < 100; i++) {
        assert(vec_push(&v, &i) == 0);
    }
    assert(vec_len(&v) == 100);

    /* Verify all values survived realloc */
    for (int i = 0; i < 100; i++) {
        int *p = (int *)vec_get(&v, (size_t)i);
        assert(*p == i);
    }

    vec_free(&v);
}

/* --- pointer arithmetic demonstration --- */

static void test_pointer_arithmetic(void) {
    int arr[5] = {10, 20, 30, 40, 50};
    int *p = arr;

    /* p + 1 advances by sizeof(int) bytes, not 1 byte */
    assert(*(p + 0) == 10);
    assert(*(p + 1) == 20);
    assert(*(p + 4) == 50);

    /* array indexing is pointer arithmetic sugar */
    assert(arr[3] == *(arr + 3));
    assert(arr[3] == *(p + 3));
}

static void test_const_pointer(void) {
    int x = 10, y = 20;
    const int *p = &x;  /* pointer to const int: can't modify *p */
    (void)p;
    (void)y;
    /* *p = 30; would be a compile error */
    p = &y;  /* but can reassign the pointer itself */
    assert(*p == 20);
}

int main(void) {
    puts("Running 02-pointers-memory tests...");

    RUN(test_swap_int);
    RUN(test_swap_double);
    RUN(test_swap_struct);
    RUN(test_sort_ints);
    RUN(test_sort_descending);
    RUN(test_sort_already_sorted);
    RUN(test_sort_single_element);
    RUN(test_sort_empty);
    RUN(test_vec_push_and_get);
    RUN(test_vec_out_of_bounds);
    RUN(test_vec_grow);
    RUN(test_pointer_arithmetic);
    RUN(test_const_pointer);

    printf("%d/%d passed\n", g_pass, g_total);
    return g_pass == g_total ? 0 : 1;
}
