/*
 * test_build.c — Tests for mathlib and strlib.
 *
 * Verifies that separate compilation and linking work correctly.
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "mathlib.h"
#include "strlib.h"

static int g_pass = 0;
static int g_total = 0;

#define RUN(name) do { ++g_total; name(); ++g_pass; printf("[PASS] %s\n", #name); } while (0)

static void test_factorial(void) {
    assert(math_factorial(0) == 1);
    assert(math_factorial(1) == 1);
    assert(math_factorial(5) == 120);
    assert(math_factorial(10) == 3628800);
}

static void test_fibonacci(void) {
    assert(math_fibonacci(0) == 0);
    assert(math_fibonacci(1) == 1);
    assert(math_fibonacci(2) == 1);
    assert(math_fibonacci(10) == 55);
    assert(math_fibonacci(20) == 6765);
}

static void test_gcd(void) {
    assert(math_gcd(12, 8) == 4);
    assert(math_gcd(7, 13) == 1);
    assert(math_gcd(100, 25) == 25);
    assert(math_gcd(-12, 8) == 4);
}

static void test_is_prime(void) {
    assert(math_is_prime(2) == 1);
    assert(math_is_prime(3) == 1);
    assert(math_is_prime(4) == 0);
    assert(math_is_prime(17) == 1);
    assert(math_is_prime(1) == 0);
    assert(math_is_prime(0) == 0);
}

static void test_str_reverse(void) {
    char s1[] = "hello";
    str_reverse(s1);
    assert(strcmp(s1, "olleh") == 0);

    char s2[] = "a";
    str_reverse(s2);
    assert(strcmp(s2, "a") == 0);

    char s3[] = "";
    str_reverse(s3);
    assert(strcmp(s3, "") == 0);
}

static void test_str_to_upper(void) {
    char s[] = "hello World 123";
    str_to_upper(s);
    assert(strcmp(s, "HELLO WORLD 123") == 0);
}

static void test_str_count_char(void) {
    assert(str_count_char("hello", 'l') == 2);
    assert(str_count_char("hello", 'z') == 0);
    assert(str_count_char("", 'a') == 0);
}

int main(void) {
    puts("Running 08-build-systems tests...");

    RUN(test_factorial);
    RUN(test_fibonacci);
    RUN(test_gcd);
    RUN(test_is_prime);
    RUN(test_str_reverse);
    RUN(test_str_to_upper);
    RUN(test_str_count_char);

    printf("%d/%d passed\n", g_pass, g_total);
    return g_pass == g_total ? 0 : 1;
}
