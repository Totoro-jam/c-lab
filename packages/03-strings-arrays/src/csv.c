/*
 * csv.c — Simple CSV line parser.
 *
 * Handles:
 *  - Comma-separated values
 *  - Quoted fields (double quotes)
 *  - Escaped quotes ("" inside quoted field)
 */
#include <stdlib.h>
#include <string.h>

#include "csv.h"

/* Internal: append a field to the dynamic array */
static int append_field(char ***fields, size_t *count, size_t *cap,
                        const char *start, size_t len) {
    if (*count == *cap) {
        size_t new_cap = *cap == 0 ? 4 : *cap * 2;
        char **new_fields = realloc(*fields, new_cap * sizeof(char *));
        if (new_fields == NULL) {
            return -1;
        }
        *fields = new_fields;
        *cap = new_cap;
    }

    char *field = malloc(len + 1);
    if (field == NULL) {
        return -1;
    }
    memcpy(field, start, len);
    field[len] = '\0';

    (*fields)[*count] = field;
    (*count)++;
    return 0;
}

char **csv_parse_line(const char *line, size_t *out_count) {
    char **fields = NULL;
    size_t count = 0;
    size_t cap = 0;
    *out_count = 0;

    const char *p = line;

    while (*p != '\0' && *p != '\n' && *p != '\r') {
        if (*p == '"') {
            /* Quoted field */
            p++; /* skip opening quote */
            /* Build unescaped content in a buffer */
            size_t buf_cap = 32;
            size_t buf_len = 0;
            char *buf = malloc(buf_cap);
            if (buf == NULL) {
                csv_free_fields(fields, count);
                return NULL;
            }

            while (*p != '\0') {
                if (*p == '"') {
                    if (*(p + 1) == '"') {
                        /* Escaped quote */
                        if (buf_len + 1 >= buf_cap) {
                            buf_cap *= 2;
                            char *new_buf = realloc(buf, buf_cap);
                            if (new_buf == NULL) {
                                free(buf);
                                csv_free_fields(fields, count);
                                return NULL;
                            }
                            buf = new_buf;
                        }
                        buf[buf_len++] = '"';
                        p += 2;
                    } else {
                        /* End of quoted field */
                        p++;
                        break;
                    }
                } else {
                    if (buf_len + 1 >= buf_cap) {
                        buf_cap *= 2;
                        char *new_buf = realloc(buf, buf_cap);
                        if (new_buf == NULL) {
                            free(buf);
                            csv_free_fields(fields, count);
                            return NULL;
                        }
                        buf = new_buf;
                    }
                    buf[buf_len++] = *p;
                    p++;
                }
            }

            if (append_field(&fields, &count, &cap, buf, buf_len) != 0) {
                free(buf);
                csv_free_fields(fields, count);
                return NULL;
            }
            free(buf);

            /* Skip comma after quoted field */
            if (*p == ',') {
                p++;
            }
        } else {
            /* Unquoted field */
            const char *start = p;
            while (*p != '\0' && *p != ',' && *p != '\n' && *p != '\r') {
                p++;
            }
            if (append_field(&fields, &count, &cap, start, (size_t)(p - start)) != 0) {
                csv_free_fields(fields, count);
                return NULL;
            }
            if (*p == ',') {
                p++;
            }
        }
    }

    /* Handle trailing comma (empty last field) */
    if (line[0] != '\0') {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == ',') {
            if (append_field(&fields, &count, &cap, "", 0) != 0) {
                csv_free_fields(fields, count);
                return NULL;
            }
        }
    }

    *out_count = count;
    return fields;
}

void csv_free_fields(char **fields, size_t count) {
    if (fields == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        free(fields[i]);
    }
    free(fields);
}
