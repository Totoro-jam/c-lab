/*
 * test_structs.c — Tests for variant, bit fields, opaque structs.
 */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "variant.h"

static int g_pass = 0;
static int g_total = 0;

#define RUN(name) do { ++g_total; name(); ++g_pass; printf("[PASS] %s\n", #name); } while (0)

/* --- Variant tests --- */

static void test_variant_int(void) {
    variant_t v = variant_int(42);
    assert(variant_kind(&v) == VAR_INT);
    long val;
    assert(variant_get_int(&v, &val) == 0);
    assert(val == 42);

    /* Type mismatch should fail */
    double d;
    assert(variant_get_double(&v, &d) == -1);
    variant_destroy(&v);
}

static void test_variant_double(void) {
    variant_t v = variant_double(3.14);
    assert(variant_kind(&v) == VAR_DOUBLE);
    double val;
    assert(variant_get_double(&v, &val) == 0);
    assert(val == 3.14);
    variant_destroy(&v);
}

static void test_variant_string(void) {
    variant_t v = variant_string("hello");
    assert(variant_kind(&v) == VAR_STRING);
    const char *s;
    assert(variant_get_string(&v, &s) == 0);
    assert(strcmp(s, "hello") == 0);

    /* Type mismatch */
    long i;
    assert(variant_get_int(&v, &i) == -1);

    variant_destroy(&v); /* must free the string */
}

static void test_variant_bool(void) {
    variant_t v = variant_bool(true);
    assert(variant_kind(&v) == VAR_BOOL);
    bool val;
    assert(variant_get_bool(&v, &val) == 0);
    assert(val == true);
    variant_destroy(&v);
}

/* --- Struct padding / offsetof tests --- */

static void test_struct_padding(void) {
    /* Demonstrate padding: char + int typically has 3 bytes padding */
    struct padded {
        char c;
        int i;
    };
    /* Size should be >= 5 (typically 8 on most platforms) */
    assert(sizeof(struct padded) >= 5);
    /* offsetof(i) is typically 4, not 1 */
    assert(offsetof(struct padded, i) >= 1);

    /* Struct with no padding needed */
    struct packed_ish {
        int a;
        int b;
    };
    assert(sizeof(struct packed_ish) == 2 * sizeof(int));
}

static void test_struct_copy_semantics(void) {
    /* Struct assignment is a shallow copy */
    struct point { int x; int y; };
    struct point a = {.x = 1, .y = 2};
    struct point b = a; /* copy */
    b.x = 99;
    assert(a.x == 1); /* original unchanged */
    assert(b.x == 99);
}

static void test_designated_init(void) {
    /* C99 designated initializers */
    struct rgb { unsigned char r, g, b; };
    struct rgb red = {.r = 255, .g = 0, .b = 0};
    assert(red.r == 255);
    assert(red.g == 0);
    assert(red.b == 0);
}

/* --- Bit field tests --- */

static void test_bit_fields(void) {
    flags_t f = {0};
    f.is_active = 1;
    f.priority = 5;
    f.category = 12;

    assert(f.is_active == 1);
    assert(f.priority == 5);
    assert(f.category == 12);

    /* Total size should be 4 bytes (32 bits total) */
    assert(sizeof(flags_t) == 4);
}

/* --- Opaque struct tests --- */

static void test_opaque_handle(void) {
    opaque_handle_t *h = opaque_create(10, 20);
    assert(h != NULL);
    assert(opaque_get_x(h) == 10);
    assert(opaque_get_y(h) == 20);
    opaque_destroy(h);
}

/* --- Union basics --- */

static void test_union_size(void) {
    /* Union size equals its largest member */
    union mixed {
        char c;
        int i;
        double d;
    };
    assert(sizeof(union mixed) == sizeof(double));
}

int main(void) {
    puts("Running 04-structs-unions tests...");

    RUN(test_variant_int);
    RUN(test_variant_double);
    RUN(test_variant_string);
    RUN(test_variant_bool);
    RUN(test_struct_padding);
    RUN(test_struct_copy_semantics);
    RUN(test_designated_init);
    RUN(test_bit_fields);
    RUN(test_opaque_handle);
    RUN(test_union_size);

    printf("%d/%d passed\n", g_pass, g_total);
    return g_pass == g_total ? 0 : 1;
}
