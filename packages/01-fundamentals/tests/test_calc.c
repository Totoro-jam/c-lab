/*
 * test_calc.c — 最朴素的 C 测试。
 *
 * 没有任何框架：每个测试是一个 static 函数，
 * 用 assert() 断言；失败 → abort() → exit code != 0。
 * 第 11 章会引入 unity / cmocka。
 */
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "calc.h"

static int g_pass = 0;
static int g_total = 0;

/* 包一层让 PASS/FAIL 输出统一 */
#define RUN(name) do { ++g_total; name(); ++g_pass; printf("[PASS] %s\n", #name); } while (0)

static void add_positive(void) {
    assert(calc_add(1, 2) == 3);
}

static void add_with_zero(void) {
    assert(calc_add(5, 0) == 5);
    assert(calc_add(0, 5) == 5);
}

static void add_negatives(void) {
    assert(calc_add(-3, -4) == -7);
    assert(calc_add(-3, 4) == 1);
}

static void divide_normal(void) {
    bool ok;
    assert(calc_divide(10, 2, &ok) == 5);
    assert(ok == true);
}

static void divide_by_zero_returns_error(void) {
    bool ok = true;
    long r = calc_divide(1, 0, &ok);
    assert(ok == false);
    assert(r == 0);
}

static void divide_ok_null_is_allowed(void) {
    /* 调用方不关心错误时，可以传 NULL */
    assert(calc_divide(10, 2, NULL) == 5);
}

static void is_even_basic(void) {
    assert(calc_is_even(2)  == true);
    assert(calc_is_even(0)  == true);
    assert(calc_is_even(3)  == false);
    assert(calc_is_even(-4) == true);
    assert(calc_is_even(-3) == false);
}

int main(void) {
    puts("Running tests...");

    RUN(add_positive);
    RUN(add_with_zero);
    RUN(add_negatives);
    RUN(divide_normal);
    RUN(divide_by_zero_returns_error);
    RUN(divide_ok_null_is_allowed);
    RUN(is_even_basic);

    printf("%d/%d passed\n", g_pass, g_total);
    return g_pass == g_total ? 0 : 1;
}
