/*
 * test_ds.c — Tests for vec, list, and hashmap.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"
#include "list.h"
#include "hashmap.h"

static int g_pass = 0;
static int g_total = 0;

#define RUN(name) do { ++g_total; name(); ++g_pass; printf("[PASS] %s\n", #name); } while (0)

/* --- Vec tests --- */

static void test_vec_basic(void) {
    ds_vec_t v;
    assert(ds_vec_init(&v) == 0);

    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        assert(ds_vec_push(&v, &values[i]) == 0);
    }
    assert(ds_vec_len(&v) == 5);

    for (int i = 0; i < 5; i++) {
        int *p = (int *)ds_vec_get(&v, (size_t)i);
        assert(*p == values[i]);
    }

    ds_vec_free(&v);
}

static void test_vec_pop(void) {
    ds_vec_t v;
    assert(ds_vec_init(&v) == 0);

    int a = 1, b = 2, c = 3;
    ds_vec_push(&v, &a);
    ds_vec_push(&v, &b);
    ds_vec_push(&v, &c);

    int *p = (int *)ds_vec_pop(&v);
    assert(*p == 3);
    assert(ds_vec_len(&v) == 2);

    p = (int *)ds_vec_pop(&v);
    assert(*p == 2);

    ds_vec_free(&v);
}

static void test_vec_grow(void) {
    ds_vec_t v;
    assert(ds_vec_init(&v) == 0);

    int values[200];
    for (int i = 0; i < 200; i++) {
        values[i] = i * 3;
        assert(ds_vec_push(&v, &values[i]) == 0);
    }
    assert(ds_vec_len(&v) == 200);

    for (int i = 0; i < 200; i++) {
        int *p = (int *)ds_vec_get(&v, (size_t)i);
        assert(*p == i * 3);
    }

    ds_vec_free(&v);
}

static void test_vec_out_of_bounds(void) {
    ds_vec_t v;
    ds_vec_init(&v);
    assert(ds_vec_get(&v, 0) == NULL);
    assert(ds_vec_pop(&v) == NULL);
    ds_vec_free(&v);
}

/* --- List tests --- */

static void test_list_push_front(void) {
    ds_list_t list;
    ds_list_init(&list);

    int a = 1, b = 2, c = 3;
    ds_list_push_front(&list, &a);
    ds_list_push_front(&list, &b);
    ds_list_push_front(&list, &c);

    assert(ds_list_len(&list) == 3);
    /* Order: c, b, a */
    assert(*(int *)ds_list_get(&list, 0) == 3);
    assert(*(int *)ds_list_get(&list, 1) == 2);
    assert(*(int *)ds_list_get(&list, 2) == 1);

    ds_list_free(&list);
}

static void test_list_push_back(void) {
    ds_list_t list;
    ds_list_init(&list);

    int a = 10, b = 20, c = 30;
    ds_list_push_back(&list, &a);
    ds_list_push_back(&list, &b);
    ds_list_push_back(&list, &c);

    assert(ds_list_len(&list) == 3);
    assert(*(int *)ds_list_get(&list, 0) == 10);
    assert(*(int *)ds_list_get(&list, 1) == 20);
    assert(*(int *)ds_list_get(&list, 2) == 30);

    ds_list_free(&list);
}

static void test_list_pop_front(void) {
    ds_list_t list;
    ds_list_init(&list);

    int a = 5, b = 10;
    ds_list_push_back(&list, &a);
    ds_list_push_back(&list, &b);

    int *p = (int *)ds_list_pop_front(&list);
    assert(*p == 5);
    assert(ds_list_len(&list) == 1);

    p = (int *)ds_list_pop_front(&list);
    assert(*p == 10);
    assert(ds_list_len(&list) == 0);
    assert(ds_list_pop_front(&list) == NULL);

    ds_list_free(&list);
}

/* --- Hashmap tests --- */

static void test_hashmap_put_get(void) {
    ds_hashmap_t map;
    assert(ds_hashmap_init(&map, 16) == 0);

    int v1 = 100, v2 = 200, v3 = 300;
    assert(ds_hashmap_put(&map, "alpha", &v1) == 0);
    assert(ds_hashmap_put(&map, "beta", &v2) == 0);
    assert(ds_hashmap_put(&map, "gamma", &v3) == 0);

    assert(ds_hashmap_len(&map) == 3);
    assert(*(int *)ds_hashmap_get(&map, "alpha") == 100);
    assert(*(int *)ds_hashmap_get(&map, "beta") == 200);
    assert(*(int *)ds_hashmap_get(&map, "gamma") == 300);
    assert(ds_hashmap_get(&map, "delta") == NULL);

    ds_hashmap_free(&map);
}

static void test_hashmap_update(void) {
    ds_hashmap_t map;
    ds_hashmap_init(&map, 4);

    int v1 = 1, v2 = 2;
    ds_hashmap_put(&map, "key", &v1);
    assert(*(int *)ds_hashmap_get(&map, "key") == 1);

    ds_hashmap_put(&map, "key", &v2); /* update */
    assert(*(int *)ds_hashmap_get(&map, "key") == 2);
    assert(ds_hashmap_len(&map) == 1); /* no duplicate */

    ds_hashmap_free(&map);
}

static void test_hashmap_remove(void) {
    ds_hashmap_t map;
    ds_hashmap_init(&map, 4);

    int v = 42;
    ds_hashmap_put(&map, "rm_me", &v);
    assert(ds_hashmap_len(&map) == 1);

    assert(ds_hashmap_remove(&map, "rm_me") == 0);
    assert(ds_hashmap_len(&map) == 0);
    assert(ds_hashmap_get(&map, "rm_me") == NULL);

    /* Remove nonexistent */
    assert(ds_hashmap_remove(&map, "nope") == -1);

    ds_hashmap_free(&map);
}

static void test_hashmap_many_entries(void) {
    ds_hashmap_t map;
    ds_hashmap_init(&map, 4); /* small bucket count to force collisions */

    int values[50];
    char keys[50][16];
    for (int i = 0; i < 50; i++) {
        values[i] = i * 7;
        snprintf(keys[i], sizeof(keys[i]), "key_%d", i);
        assert(ds_hashmap_put(&map, keys[i], &values[i]) == 0);
    }
    assert(ds_hashmap_len(&map) == 50);

    for (int i = 0; i < 50; i++) {
        int *p = (int *)ds_hashmap_get(&map, keys[i]);
        assert(p != NULL);
        assert(*p == i * 7);
    }

    ds_hashmap_free(&map);
}

int main(void) {
    puts("Running 06-data-structures tests...");

    RUN(test_vec_basic);
    RUN(test_vec_pop);
    RUN(test_vec_grow);
    RUN(test_vec_out_of_bounds);
    RUN(test_list_push_front);
    RUN(test_list_push_back);
    RUN(test_list_pop_front);
    RUN(test_hashmap_put_get);
    RUN(test_hashmap_update);
    RUN(test_hashmap_remove);
    RUN(test_hashmap_many_entries);

    printf("%d/%d passed\n", g_pass, g_total);
    return g_pass == g_total ? 0 : 1;
}
