/*
 * test_fileio.c — Tests for file I/O utilities.
 *
 * Uses tmpfile/tmpnam patterns for test isolation.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "fileutil.h"

static int g_pass = 0;
static int g_total = 0;

#define RUN(name) do { ++g_total; name(); ++g_pass; printf("[PASS] %s\n", #name); } while (0)

/* Helper to create a temp file with given content */
static const char *write_temp(const char *content) {
    static char path[256];
    snprintf(path, sizeof(path), "/tmp/c_lab_test_%d.tmp", g_total);
    FILE *f = fopen(path, "w");
    assert(f != NULL);
    fputs(content, f);
    fclose(f);
    return path;
}

static void cleanup_temp(const char *path) {
    remove(path);
}

/* --- Tests --- */

static void test_count_lines_basic(void) {
    const char *path = write_temp("line1\nline2\nline3\n");
    long n = file_count_lines(path);
    assert(n == 3);
    cleanup_temp(path);
}

static void test_count_lines_no_trailing_newline(void) {
    const char *path = write_temp("line1\nline2");
    long n = file_count_lines(path);
    assert(n == 2);
    cleanup_temp(path);
}

static void test_count_lines_empty(void) {
    const char *path = write_temp("");
    long n = file_count_lines(path);
    assert(n == 0);
    cleanup_temp(path);
}

static void test_count_lines_nonexistent(void) {
    long n = file_count_lines("/tmp/nonexistent_file_xyz.txt");
    assert(n == -1);
}

static void test_read_all(void) {
    const char *content = "hello, file I/O!";
    const char *path = write_temp(content);
    size_t size = 0;
    char *buf = file_read_all(path, &size);
    assert(buf != NULL);
    assert(size == strlen(content));
    assert(strcmp(buf, content) == 0);
    free(buf);
    cleanup_temp(path);
}

static void test_write_and_read_all(void) {
    const char *path = "/tmp/c_lab_test_write.tmp";
    const char data[] = "binary\x00data\x01here";
    size_t data_len = sizeof(data) - 1;

    assert(file_write_all(path, data, data_len) == 0);

    size_t size = 0;
    char *buf = file_read_all(path, &size);
    assert(buf != NULL);
    assert(size == data_len);
    assert(memcmp(buf, data, data_len) == 0);
    free(buf);
    remove(path);
}

static void test_records_roundtrip(void) {
    const char *path = "/tmp/c_lab_test_records.bin";

    record_t input[3] = {
        {.id = 1, .value = 3.14, .name = "alpha"},
        {.id = 2, .value = 2.71, .name = "beta"},
        {.id = 3, .value = 1.41, .name = "gamma"},
    };

    assert(records_write(path, input, 3) == 0);

    size_t count = 0;
    record_t *output = records_read(path, &count);
    assert(output != NULL);
    assert(count == 3);

    for (size_t i = 0; i < 3; i++) {
        assert(output[i].id == input[i].id);
        assert(output[i].value == input[i].value);
        assert(strcmp(output[i].name, input[i].name) == 0);
    }

    free(output);
    remove(path);
}

static void test_errno_on_open_failure(void) {
    errno = 0;
    char *buf = file_read_all("/tmp/definitely_does_not_exist_xyz.bin", &(size_t){0});
    assert(buf == NULL);
    assert(errno != 0); /* errno should be set (ENOENT) */
}

int main(void) {
    puts("Running 05-file-io tests...");

    RUN(test_count_lines_basic);
    RUN(test_count_lines_no_trailing_newline);
    RUN(test_count_lines_empty);
    RUN(test_count_lines_nonexistent);
    RUN(test_read_all);
    RUN(test_write_and_read_all);
    RUN(test_records_roundtrip);
    RUN(test_errno_on_open_failure);

    printf("%d/%d passed\n", g_pass, g_total);
    return g_pass == g_total ? 0 : 1;
}
