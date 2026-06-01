#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db_demo.h"
#include "http_demo.h"

static int passed = 0;
static int failed = 0;

#define RUN(fn)                                             \
    do {                                                    \
        printf("  ");                                       \
        if (fn()) { printf("[PASS] %s\n", #fn); passed++; } \
        else      { printf("[FAIL] %s\n", #fn); failed++; } \
    } while (0)

#define SKIP(fn, reason)                                    \
    do { printf("  [SKIP] %s (%s)\n", #fn, reason); } while (0)

/* ── SQLite tests (in-memory, no file IO) ── */

static int test_db_open_close(void)
{
    db_handle_t h = {0};
    if (db_open(&h, ":memory:") != SQLITE_OK) return 0;
    db_close(&h);
    return 1;
}

static int test_db_create_table(void)
{
    db_handle_t h = {0};
    db_open(&h, ":memory:");
    int rc = db_create_table(&h);
    db_close(&h);
    return rc == SQLITE_OK;
}

static int test_db_insert_and_get(void)
{
    db_handle_t h = {0};
    db_open(&h, ":memory:");
    db_create_table(&h);

    db_insert_user(&h, "Alice", 30);
    db_user_t u = {0};
    int rc = db_get_user(&h, 1, &u);

    db_close(&h);
    return rc == SQLITE_OK
        && u.id == 1
        && strcmp(u.name, "Alice") == 0
        && u.age == 30;
}

static int test_db_count(void)
{
    db_handle_t h = {0};
    db_open(&h, ":memory:");
    db_create_table(&h);

    db_insert_user(&h, "Alice", 30);
    db_insert_user(&h, "Bob", 25);
    int count = db_count_users(&h);

    db_close(&h);
    return count == 2;
}

static int test_db_delete(void)
{
    db_handle_t h = {0};
    db_open(&h, ":memory:");
    db_create_table(&h);

    db_insert_user(&h, "Alice", 30);
    db_insert_user(&h, "Bob", 25);
    db_delete_user(&h, 1);
    int count = db_count_users(&h);

    db_user_t u = {0};
    int rc = db_get_user(&h, 1, &u);

    db_close(&h);
    return count == 1 && rc != SQLITE_OK;
}

static int test_db_get_nonexistent(void)
{
    db_handle_t h = {0};
    db_open(&h, ":memory:");
    db_create_table(&h);

    db_user_t u = {0};
    int rc = db_get_user(&h, 999, &u);

    db_close(&h);
    return rc != SQLITE_OK;
}

/* ── libcurl tests ── */

static int test_http_response_lifecycle(void)
{
    http_response_t resp;
    http_response_init(&resp);
    int ok = (resp.data == NULL && resp.len == 0 && resp.cap == 0);
    http_response_free(&resp);
    return ok;
}

static int test_http_get_success(void)
{
    http_response_t resp;
    http_response_init(&resp);

    int rc = http_get("http://httpbin.org/get", &resp);
    int ok = (rc == 0 && resp.len > 0 && resp.data != NULL);

    http_response_free(&resp);
    return ok;
}

static int test_http_get_bad_url(void)
{
    http_response_t resp;
    http_response_init(&resp);

    int rc = http_get("http://this-domain-does-not-exist.invalid/", &resp);
    int ok = (rc != 0);

    http_response_free(&resp);
    return ok;
}

/* ── main ── */

int main(void)
{
    printf("Running 12-popular-libs tests...\n");

    printf("\n[SQLite]\n");
    RUN(test_db_open_close);
    RUN(test_db_create_table);
    RUN(test_db_insert_and_get);
    RUN(test_db_count);
    RUN(test_db_delete);
    RUN(test_db_get_nonexistent);

    printf("\n[libcurl]\n");
    RUN(test_http_response_lifecycle);

    /* network tests — skip if SKIP_NETWORK is set */
    const char *skip_net = getenv("SKIP_NETWORK");
    if (skip_net && skip_net[0] == '1') {
        SKIP(test_http_get_success, "SKIP_NETWORK=1");
        SKIP(test_http_get_bad_url, "SKIP_NETWORK=1");
    } else {
        RUN(test_http_get_success);
        RUN(test_http_get_bad_url);
    }

    printf("\n%d/%d passed\n", passed, passed + failed);
    return failed > 0 ? 1 : 0;
}
