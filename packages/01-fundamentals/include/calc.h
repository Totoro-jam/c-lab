/*
 * calc.h — 公开接口（声明）。
 *
 * 头文件只放声明 + 必要的 typedef / macro。
 * 实现放在 src/calc.c。
 */
#ifndef CALC_H
#define CALC_H

#include <stdbool.h>

/*
 * 返回 a + b。整数加法，可能溢出（这里演示用，不处理）。
 */
long calc_add(long a, long b);

/*
 * 返回 a / b。
 * b == 0 时返回 0 并设置 *ok = false；成功设置 *ok = true。
 * 也可以传 ok = NULL 表示不关心错误。
 *
 * 设计取舍：C 没有 Result 类型，"出参 + bool" 是常见模式。
 */
long calc_divide(long a, long b, bool *ok);

/*
 * n 是不是偶数。负数也按数学定义处理：-4 是偶数。
 */
bool calc_is_even(long n);

#endif /* CALC_H */
