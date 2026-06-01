/*
 * fileutil.c — File I/O implementations.
 *
 * Key patterns:
 *  - Always check fopen return
 *  - Use fread return value, not feof, for loop control
 *  - Close files in all code paths (including error paths)
 *  - Binary mode "rb"/"wb" for struct I/O
 */
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "fileutil.h"

long file_count_lines(const char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return -1;
    }

    long count = 0;
    int ch;

    while ((ch = fgetc(f)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }

    if (ferror(f)) {
        fclose(f);
        return -1;
    }

    /* Count last line if it doesn't end with newline */
    if (count == 0 && ftell(f) > 0) {
        count = 1;
    } else {
        /* Check if file ended without trailing newline */
        if (fseek(f, -1, SEEK_END) == 0) {
            int last = fgetc(f);
            if (last != '\n' && last != EOF) {
                count++;
            }
        }
    }

    fclose(f);
    return count;
}

char *file_read_all(const char *path, size_t *out_size) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return NULL;
    }

    /* Get file size */
    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return NULL;
    }
    long size = ftell(f);
    if (size < 0) {
        fclose(f);
        return NULL;
    }
    rewind(f);

    char *buf = malloc((size_t)size + 1);
    if (buf == NULL) {
        fclose(f);
        return NULL;
    }

    size_t read = fread(buf, 1, (size_t)size, f);
    if (ferror(f)) {
        free(buf);
        fclose(f);
        return NULL;
    }

    buf[read] = '\0';
    *out_size = read;
    fclose(f);
    return buf;
}

int file_write_all(const char *path, const void *data, size_t size) {
    FILE *f = fopen(path, "wb");
    if (f == NULL) {
        return -1;
    }

    size_t written = fwrite(data, 1, size, f);
    if (written != size) {
        fclose(f);
        return -1;
    }

    fclose(f);
    return 0;
}

int records_write(const char *path, const record_t *records, size_t count) {
    FILE *f = fopen(path, "wb");
    if (f == NULL) {
        return -1;
    }

    /* Write count header */
    if (fwrite(&count, sizeof(count), 1, f) != 1) {
        fclose(f);
        return -1;
    }

    /* Write records */
    if (fwrite(records, sizeof(record_t), count, f) != count) {
        fclose(f);
        return -1;
    }

    fclose(f);
    return 0;
}

record_t *records_read(const char *path, size_t *out_count) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return NULL;
    }

    size_t count;
    if (fread(&count, sizeof(count), 1, f) != 1) {
        fclose(f);
        return NULL;
    }

    record_t *records = malloc(count * sizeof(record_t));
    if (records == NULL) {
        fclose(f);
        return NULL;
    }

    if (fread(records, sizeof(record_t), count, f) != count) {
        free(records);
        fclose(f);
        return NULL;
    }

    *out_count = count;
    fclose(f);
    return records;
}
