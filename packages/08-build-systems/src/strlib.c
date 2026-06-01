/*
 * strlib.c — String library implementation.
 */
#include <ctype.h>
#include <string.h>
#include "strlib.h"

void str_reverse(char *s) {
    size_t len = strlen(s);
    if (len <= 1) return;
    char *left = s;
    char *right = s + len - 1;
    while (left < right) {
        char tmp = *left;
        *left = *right;
        *right = tmp;
        left++;
        right--;
    }
}

void str_to_upper(char *s) {
    for (; *s; s++) {
        *s = (char)toupper((unsigned char)*s);
    }
}

size_t str_count_char(const char *s, char c) {
    size_t count = 0;
    for (; *s; s++) {
        if (*s == c) count++;
    }
    return count;
}
