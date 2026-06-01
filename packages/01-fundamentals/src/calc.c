/*
 * calc.c — calc.h 的实现。
 *
 * 注意几个工程化习惯：
 *  - 包含自己的头文件（让编译器帮你校验签名一致）
 *  - 错误通过出参 ok 报告，函数返回值不混淆
 *  - 对负数取模 C99+ 保证符号与被除数同
 */
#include <stddef.h>   /* NULL */

#include "calc.h"

long calc_add(long a, long b) {
    return a + b;
}

long calc_divide(long a, long b, bool *ok) {
    if (b == 0) {
        if (ok != NULL) {
            *ok = false;
        }
        return 0;
    }
    if (ok != NULL) {
        *ok = true;
    }
    return a / b;
}

bool calc_is_even(long n) {
    /* C99+ : n % 2 在 n < 0 时可能是 -1，所以判 == 0 而不是 == 1 */
    return (n % 2) == 0;
}
