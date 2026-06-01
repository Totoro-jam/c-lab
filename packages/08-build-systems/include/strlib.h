/*
 * strlib.h — Simple string library to demonstrate multi-file projects.
 */
#ifndef STRLIB_H
#define STRLIB_H

#include <stddef.h>

/*
 * Reverse a string in place.
 */
void str_reverse(char *s);

/*
 * Convert string to uppercase in place.
 */
void str_to_upper(char *s);

/*
 * Count occurrences of a character in a string.
 */
size_t str_count_char(const char *s, char c);

#endif /* STRLIB_H */
