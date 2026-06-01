/*
 * test_preprocessor.c — Tests for macro patterns and X-macros.
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "macros.h"
#include "xmacro.h"

static int g_pass = 0;
static int g_total = 0;

#define RUN(name) do { ++g_total; name(); ++g_pass; printf("[PASS] %s\n", #name); } while (0)

/* --- Macro tests --- */

static void test_square(void) {
    assert(SQUARE(3) == 9);
    assert(SQUARE(0) == 0);
    /* This is the key test: 1+2 should give 9, not 5 */
    assert(SQUARE(1 + 2) == 9);
}

static void test_max_min(void) {
    assert(MAX(3, 5) == 5);
    assert(MAX(10, 2) == 10);
    assert(MIN(3, 5) == 3);
    assert(MIN(-1, 1) == -1);
}

static void test_clamp(void) {
    assert(CLAMP(5, 0, 10) == 5);
    assert(CLAMP(-5, 0, 10) == 0);
    assert(CLAMP(15, 0, 10) == 10);
}

static void test_array_len(void) {
    int arr[7] = {0};
    assert(ARRAY_LEN(arr) == 7);

    double d[3] = {0};
    assert(ARRAY_LEN(d) == 3);
}

static void test_swap_int_macro(void) {
    int a = 10, b = 20;
    SWAP_INT(a, b);
    assert(a == 20);
    assert(b == 10);
}

static void test_stringify(void) {
    /* STRINGIFY turns the token directly into a string */
    assert(strcmp(STRINGIFY(hello), "hello") == 0);

    /* XSTRINGIFY expands macros first */
    assert(strcmp(XSTRINGIFY(VERSION_MAJOR), "1") == 0);
}

static void test_concat(void) {
    int xy = 42;
    /* CONCAT(x, y) becomes the token xy */
    assert(CONCAT(x, y) == 42);
}

static void test_static_assert(void) {
    /* These compile-time assertions pass at compile time */
    STATIC_ASSERT(sizeof(int) >= 4, "int must be at least 4 bytes");
    STATIC_ASSERT(sizeof(char) == 1, "char must be 1 byte");
    /* If either fails, compilation fails — that IS the test */
}

/* --- X-macro tests --- */

static void test_color_enum_values(void) {
    /* Enum values are sequential starting from 0 */
    assert(COLOR_RED == 0);
    assert(COLOR_GREEN == 1);
    assert(COLOR_BLUE == 2);
    assert(COLOR_COUNT == 6);
}

static void test_color_name(void) {
    assert(strcmp(color_name(COLOR_RED), "RED") == 0);
    assert(strcmp(color_name(COLOR_BLUE), "BLUE") == 0);
    assert(strcmp(color_name(COLOR_MAGENTA), "MAGENTA") == 0);
    assert(strcmp(color_name(COLOR_COUNT), "UNKNOWN") == 0);
}

static void test_color_parse(void) {
    color_t c;
    assert(color_parse("RED", &c) == 0);
    assert(c == COLOR_RED);

    assert(color_parse("CYAN", &c) == 0);
    assert(c == COLOR_CYAN);

    assert(color_parse("PURPLE", &c) == -1); /* not in list */
}

int main(void) {
    puts("Running 07-preprocessor-headers tests...");

    RUN(test_square);
    RUN(test_max_min);
    RUN(test_clamp);
    RUN(test_array_len);
    RUN(test_swap_int_macro);
    RUN(test_stringify);
    RUN(test_concat);
    RUN(test_static_assert);
    RUN(test_color_enum_values);
    RUN(test_color_name);
    RUN(test_color_parse);

    printf("%d/%d passed\n", g_pass, g_total);
    return g_pass == g_total ? 0 : 1;
}
