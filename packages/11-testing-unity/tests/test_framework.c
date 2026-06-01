/*
 * test_framework.c — Demonstrates the minitest framework
 * by testing the vec and list data structures.
 *
 * Compared to raw assert:
 *  - Colored output
 *  - Continues after failure
 *  - Reports file/line of failure
 *  - setUp/tearDown lifecycle
 */
#include "minitest.h"
#include "vec.h"
#include "list.h"

/* --- setUp / tearDown for vec tests --- */

static tvec_t g_vec;

static void vec_setup(void) {
    tvec_init(&g_vec);
}

static void vec_teardown(void) {
    tvec_free(&g_vec);
}

/* --- Vec tests using the framework --- */

static void test_vec_init(void) {
    MT_ASSERT_EQ_INT(0, tvec_len(&g_vec));
}

static void test_vec_push(void) {
    int a = 10, b = 20;
    tvec_push(&g_vec, &a);
    tvec_push(&g_vec, &b);
    MT_ASSERT_EQ_INT(2, tvec_len(&g_vec));
}

static void test_vec_get(void) {
    int a = 42;
    tvec_push(&g_vec, &a);
    int *p = (int *)tvec_get(&g_vec, 0);
    MT_ASSERT_NOT_NULL(p);
    MT_ASSERT_EQ_INT(42, *p);
}

static void test_vec_get_out_of_bounds(void) {
    MT_ASSERT_NULL(tvec_get(&g_vec, 0));
    MT_ASSERT_NULL(tvec_get(&g_vec, 100));
}

static void test_vec_pop(void) {
    int a = 1, b = 2, c = 3;
    tvec_push(&g_vec, &a);
    tvec_push(&g_vec, &b);
    tvec_push(&g_vec, &c);

    int *p = (int *)tvec_pop(&g_vec);
    MT_ASSERT_NOT_NULL(p);
    MT_ASSERT_EQ_INT(3, *p);
    MT_ASSERT_EQ_INT(2, tvec_len(&g_vec));
}

static void test_vec_pop_empty(void) {
    MT_ASSERT_NULL(tvec_pop(&g_vec));
}

static void test_vec_grow_large(void) {
    int values[100];
    for (int i = 0; i < 100; i++) {
        values[i] = i;
        tvec_push(&g_vec, &values[i]);
    }
    MT_ASSERT_EQ_INT(100, tvec_len(&g_vec));

    for (int i = 0; i < 100; i++) {
        int *p = (int *)tvec_get(&g_vec, (size_t)i);
        MT_ASSERT_EQ_INT(i, *p);
    }
}

/* --- List tests --- */

static tlist_t g_list;

static void list_setup(void) {
    tlist_init(&g_list);
}

static void list_teardown(void) {
    tlist_free(&g_list);
}

static void test_list_init(void) {
    MT_ASSERT_EQ_INT(0, tlist_len(&g_list));
}

static void test_list_push_front(void) {
    int a = 1, b = 2;
    tlist_push_front(&g_list, &a);
    tlist_push_front(&g_list, &b);
    MT_ASSERT_EQ_INT(2, tlist_len(&g_list));

    /* Most recent push_front is at front */
    int *p = (int *)tlist_pop_front(&g_list);
    MT_ASSERT_EQ_INT(2, *p);
}

static void test_list_push_back(void) {
    int a = 10, b = 20, c = 30;
    tlist_push_back(&g_list, &a);
    tlist_push_back(&g_list, &b);
    tlist_push_back(&g_list, &c);
    MT_ASSERT_EQ_INT(3, tlist_len(&g_list));

    /* pop_front should give first pushed */
    int *p = (int *)tlist_pop_front(&g_list);
    MT_ASSERT_EQ_INT(10, *p);
}

static void test_list_pop_empty(void) {
    void *p = tlist_pop_front(&g_list);
    MT_ASSERT_NULL(p);
}

/* --- Main: register and run all suites --- */

int main(void) {
    int result = 0;

    /* Vec suite */
    MINITEST_DEFINE_SUITE(vec_suite, vec_setup, vec_teardown);
    MINITEST_ADD(vec_suite, test_vec_init);
    MINITEST_ADD(vec_suite, test_vec_push);
    MINITEST_ADD(vec_suite, test_vec_get);
    MINITEST_ADD(vec_suite, test_vec_get_out_of_bounds);
    MINITEST_ADD(vec_suite, test_vec_pop);
    MINITEST_ADD(vec_suite, test_vec_pop_empty);
    MINITEST_ADD(vec_suite, test_vec_grow_large);
    result |= MINITEST_RUN(vec_suite);

    /* List suite */
    MINITEST_DEFINE_SUITE(list_suite, list_setup, list_teardown);
    MINITEST_ADD(list_suite, test_list_init);
    MINITEST_ADD(list_suite, test_list_push_front);
    MINITEST_ADD(list_suite, test_list_push_back);
    MINITEST_ADD(list_suite, test_list_pop_empty);
    result |= MINITEST_RUN(list_suite);

    return result;
}
