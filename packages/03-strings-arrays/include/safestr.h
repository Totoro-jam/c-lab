/*
 * safestr.h — Safe string utilities.
 *
 * Demonstrates safe alternatives to strcpy/strcat/sprintf.
 */
#ifndef SAFESTR_H
#define SAFESTR_H

#include <stddef.h>

/*
 * Safe string copy. Always NUL-terminates.
 * Returns number of characters that would have been written (like snprintf).
 * If return >= dest_size, output was truncated.
 */
size_t safe_strcpy(char *dest, size_t dest_size, const char *src);

/*
 * Safe string concatenate. Always NUL-terminates.
 * Returns total length that would have been needed.
 */
size_t safe_strcat(char *dest, size_t dest_size, const char *src);

/*
 * Split a string by delimiter. Does NOT modify the original string.
 * Returns an array of newly allocated strings (caller must free each + the array).
 * *out_count receives the number of tokens.
 * Returns NULL on allocation failure.
 */
char **str_split(const char *str, char delim, size_t *out_count);

/*
 * Free the result of str_split.
 */
void str_split_free(char **tokens, size_t count);

#endif /* SAFESTR_H */
