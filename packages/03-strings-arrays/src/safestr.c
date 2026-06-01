/*
 * safestr.c — Safe string operations.
 *
 * Key lessons:
 *  - Always NUL-terminate
 *  - Use snprintf semantics (return "would-have-written" count)
 *  - Never use strcpy/strcat/gets in real code
 */
#include <stdlib.h>
#include <string.h>

#include "safestr.h"

size_t safe_strcpy(char *dest, size_t dest_size, const char *src) {
    if (dest_size == 0) {
        return strlen(src);
    }

    size_t src_len = strlen(src);
    size_t copy_len = src_len < dest_size - 1 ? src_len : dest_size - 1;

    memcpy(dest, src, copy_len);
    dest[copy_len] = '\0';

    return src_len;
}

size_t safe_strcat(char *dest, size_t dest_size, const char *src) {
    size_t dest_len = strlen(dest);
    size_t src_len = strlen(src);

    if (dest_len >= dest_size) {
        return dest_len + src_len;
    }

    size_t remaining = dest_size - dest_len - 1;
    size_t copy_len = src_len < remaining ? src_len : remaining;

    memcpy(dest + dest_len, src, copy_len);
    dest[dest_len + copy_len] = '\0';

    return dest_len + src_len;
}

char **str_split(const char *str, char delim, size_t *out_count) {
    *out_count = 0;

    /* Count delimiters to estimate token count */
    size_t count = 1;
    for (const char *p = str; *p; p++) {
        if (*p == delim) {
            count++;
        }
    }

    char **tokens = malloc(count * sizeof(char *));
    if (tokens == NULL) {
        return NULL;
    }

    size_t idx = 0;
    const char *start = str;

    for (const char *p = str; ; p++) {
        if (*p == delim || *p == '\0') {
            size_t len = (size_t)(p - start);
            tokens[idx] = malloc(len + 1);
            if (tokens[idx] == NULL) {
                str_split_free(tokens, idx);
                return NULL;
            }
            memcpy(tokens[idx], start, len);
            tokens[idx][len] = '\0';
            idx++;
            if (*p == '\0') {
                break;
            }
            start = p + 1;
        }
    }

    *out_count = idx;
    return tokens;
}

void str_split_free(char **tokens, size_t count) {
    if (tokens == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        free(tokens[i]);
    }
    free(tokens);
}
