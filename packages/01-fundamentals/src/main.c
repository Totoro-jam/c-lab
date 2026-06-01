/*
 * main.c — 把 calc 包装成一个 CLI。
 *
 *   ./calc add 1 2       → 3
 *   ./calc divide 10 2   → 5
 *   ./calc even 4        → true
 *
 * 演示重点：
 *  - argc/argv 怎么用
 *  - strtol 替代 atoi (atoi 不能报错)
 *  - 退出码：0 成功，非 0 失败
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calc.h"

static int parse_long(const char *s, long *out) {
    char *end;
    errno = 0;
    long v = strtol(s, &end, 10);
    if (errno != 0 || *end != '\0' || end == s) {
        return -1;
    }
    *out = v;
    return 0;
}

static int usage(void) {
    fputs(
        "usage:\n"
        "  calc add A B\n"
        "  calc divide A B\n"
        "  calc even N\n",
        stderr);
    return 2;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return usage();
    }

    const char *cmd = argv[1];

    if (strcmp(cmd, "add") == 0) {
        if (argc != 4) return usage();
        long a, b;
        if (parse_long(argv[2], &a) != 0 || parse_long(argv[3], &b) != 0) {
            fputs("error: invalid number\n", stderr);
            return 1;
        }
        printf("%ld\n", calc_add(a, b));
        return 0;
    }

    if (strcmp(cmd, "divide") == 0) {
        if (argc != 4) return usage();
        long a, b;
        if (parse_long(argv[2], &a) != 0 || parse_long(argv[3], &b) != 0) {
            fputs("error: invalid number\n", stderr);
            return 1;
        }
        bool ok;
        long r = calc_divide(a, b, &ok);
        if (!ok) {
            fputs("error: cannot divide by zero\n", stderr);
            return 1;
        }
        printf("%ld\n", r);
        return 0;
    }

    if (strcmp(cmd, "even") == 0) {
        if (argc != 3) return usage();
        long n;
        if (parse_long(argv[2], &n) != 0) {
            fputs("error: invalid number\n", stderr);
            return 1;
        }
        puts(calc_is_even(n) ? "true" : "false");
        return 0;
    }

    return usage();
}
