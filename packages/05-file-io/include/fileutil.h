/*
 * fileutil.h — File I/O utilities.
 *
 * Demonstrates: fopen/fclose, line reading, binary I/O, errno handling.
 */
#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <stddef.h>
#include <stdio.h>

/*
 * Count lines in a file. Returns -1 on error (sets errno).
 */
long file_count_lines(const char *path);

/*
 * Read entire file into a heap-allocated buffer.
 * *out_size receives the number of bytes read.
 * Returns NULL on error (sets errno).
 * Caller must free() the returned buffer.
 */
char *file_read_all(const char *path, size_t *out_size);

/*
 * Write a buffer to a file (binary mode).
 * Returns 0 on success, -1 on error.
 */
int file_write_all(const char *path, const void *data, size_t size);

/*
 * A simple record for binary I/O demo.
 */
typedef struct {
    int id;
    double value;
    char name[32];
} record_t;

/*
 * Write an array of records to a binary file.
 * Returns 0 on success, -1 on error.
 */
int records_write(const char *path, const record_t *records, size_t count);

/*
 * Read records from a binary file.
 * *out_count receives number of records read.
 * Returns heap-allocated array, caller must free().
 * Returns NULL on error.
 */
record_t *records_read(const char *path, size_t *out_count);

#endif /* FILEUTIL_H */
