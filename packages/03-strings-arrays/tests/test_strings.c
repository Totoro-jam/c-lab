/*
 * test_strings.c — Tests for safe string operations and CSV parser.
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "safestr.h"
#include "csv.h"

static int g_pass = 0;
static int g_total = 0;

#define RUN(name) do { ++g_total; name(); ++g_pass; printf("[PASS] %s\n", #name); } while (0)

/* --- safe_strcpy tests --- */

static void test_strcpy_normal(void) {
    char buf[20];
    size_t r = safe_strcpy(buf, sizeof(buf), "hello");
    assert(r == 5);
    assert(strcmp(buf, "hello") == 0);
}

static void test_strcpy_truncation(void) {
    char buf[4];
    size_t r = safe_strcpy(buf, sizeof(buf), "hello world");
    assert(r == 11); /* would-have-written */
    assert(strcmp(buf, "hel") == 0);
    assert(buf[3] == '\0');
}

static void test_strcpy_empty(void) {
    char buf[10];
    size_t r = safe_strcpy(buf, sizeof(buf), "");
    assert(r == 0);
    assert(buf[0] == '\0');
}

static void test_strcpy_exact_fit(void) {
    char buf[6];
    size_t r = safe_strcpy(buf, sizeof(buf), "hello");
    assert(r == 5);
    assert(strcmp(buf, "hello") == 0);
}

/* --- safe_strcat tests --- */

static void test_strcat_normal(void) {
    char buf[20] = "hello";
    size_t r = safe_strcat(buf, sizeof(buf), " world");
    assert(r == 11);
    assert(strcmp(buf, "hello world") == 0);
}

static void test_strcat_truncation(void) {
    char buf[8] = "hello";
    size_t r = safe_strcat(buf, sizeof(buf), " world");
    assert(r == 11); /* total needed */
    assert(strcmp(buf, "hello w") == 0);
}

/* --- str_split tests --- */

static void test_split_basic(void) {
    size_t count = 0;
    char **tokens = str_split("a,b,c", ',', &count);
    assert(tokens != NULL);
    assert(count == 3);
    assert(strcmp(tokens[0], "a") == 0);
    assert(strcmp(tokens[1], "b") == 0);
    assert(strcmp(tokens[2], "c") == 0);
    str_split_free(tokens, count);
}

static void test_split_empty_fields(void) {
    size_t count = 0;
    char **tokens = str_split(",hello,", ',', &count);
    assert(tokens != NULL);
    assert(count == 3);
    assert(strcmp(tokens[0], "") == 0);
    assert(strcmp(tokens[1], "hello") == 0);
    assert(strcmp(tokens[2], "") == 0);
    str_split_free(tokens, count);
}

static void test_split_no_delimiter(void) {
    size_t count = 0;
    char **tokens = str_split("hello", ',', &count);
    assert(tokens != NULL);
    assert(count == 1);
    assert(strcmp(tokens[0], "hello") == 0);
    str_split_free(tokens, count);
}

/* --- CSV tests --- */

static void test_csv_simple(void) {
    size_t count = 0;
    char **fields = csv_parse_line("name,age,city", &count);
    assert(fields != NULL);
    assert(count == 3);
    assert(strcmp(fields[0], "name") == 0);
    assert(strcmp(fields[1], "age") == 0);
    assert(strcmp(fields[2], "city") == 0);
    csv_free_fields(fields, count);
}

static void test_csv_quoted(void) {
    size_t count = 0;
    char **fields = csv_parse_line("\"hello, world\",42", &count);
    assert(fields != NULL);
    assert(count == 2);
    assert(strcmp(fields[0], "hello, world") == 0);
    assert(strcmp(fields[1], "42") == 0);
    csv_free_fields(fields, count);
}

static void test_csv_escaped_quotes(void) {
    size_t count = 0;
    char **fields = csv_parse_line("\"say \"\"hi\"\"\",done", &count);
    assert(fields != NULL);
    assert(count == 2);
    assert(strcmp(fields[0], "say \"hi\"") == 0);
    assert(strcmp(fields[1], "done") == 0);
    csv_free_fields(fields, count);
}

/* --- array/pointer distinction test --- */

static void test_array_vs_pointer(void) {
    /* String literal is read-only, char[] is a mutable copy */
    char arr[] = "hello";
    arr[0] = 'H';
    assert(arr[0] == 'H');

    /* sizeof array includes the NUL terminator */
    assert(sizeof(arr) == 6);

    /* Multi-dimensional array addressing */
    int mat[2][3] = {{1, 2, 3}, {4, 5, 6}};
    /* mat[i][j] == *(*(mat + i) + j) */
    assert(mat[1][2] == 6);
    assert(*(*(mat + 1) + 2) == 6);
}

int main(void) {
    puts("Running 03-strings-arrays tests...");

    RUN(test_strcpy_normal);
    RUN(test_strcpy_truncation);
    RUN(test_strcpy_empty);
    RUN(test_strcpy_exact_fit);
    RUN(test_strcat_normal);
    RUN(test_strcat_truncation);
    RUN(test_split_basic);
    RUN(test_split_empty_fields);
    RUN(test_split_no_delimiter);
    RUN(test_csv_simple);
    RUN(test_csv_quoted);
    RUN(test_csv_escaped_quotes);
    RUN(test_array_vs_pointer);

    printf("%d/%d passed\n", g_pass, g_total);
    return g_pass == g_total ? 0 : 1;
}
