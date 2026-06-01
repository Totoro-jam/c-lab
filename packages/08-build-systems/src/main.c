/*
 * main.c — Demo CLI that uses both mathlib and strlib.
 *
 * Demonstrates linking against a static library.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mathlib.h"
#include "strlib.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: demo <command> [args...]\n");
        fprintf(stderr, "  demo fact N       - factorial\n");
        fprintf(stderr, "  demo fib N        - fibonacci\n");
        fprintf(stderr, "  demo prime N      - is prime?\n");
        fprintf(stderr, "  demo reverse STR  - reverse string\n");
        return 1;
    }

    if (strcmp(argv[1], "fact") == 0 && argc == 3) {
        printf("%ld\n", math_factorial(atoi(argv[2])));
    } else if (strcmp(argv[1], "fib") == 0 && argc == 3) {
        printf("%ld\n", math_fibonacci(atoi(argv[2])));
    } else if (strcmp(argv[1], "prime") == 0 && argc == 3) {
        printf("%s\n", math_is_prime(atoi(argv[2])) ? "yes" : "no");
    } else if (strcmp(argv[1], "reverse") == 0 && argc == 3) {
        char buf[256];
        snprintf(buf, sizeof(buf), "%s", argv[2]);
        str_reverse(buf);
        printf("%s\n", buf);
    } else {
        fprintf(stderr, "unknown command: %s\n", argv[1]);
        return 1;
    }

    return 0;
}
