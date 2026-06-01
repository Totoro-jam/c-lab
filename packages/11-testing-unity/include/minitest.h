/*
 * minitest.h — Minimal test framework with colored output.
 *
 * Features beyond raw assert():
 *  - Named test registration
 *  - Pass/fail counting
 *  - Colored terminal output (green PASS, red FAIL)
 *  - Descriptive failure messages with file/line
 *  - setUp/tearDown hooks
 *  - Continues running after a failure (no abort)
 *
 * Usage:
 *   MINITEST_DEFINE_SUITE(my_tests, my_setup, my_teardown);
 *   MINITEST_ADD(my_tests, test_func_name);
 *   int result = MINITEST_RUN(my_tests);
 */
#ifndef MINITEST_H
#define MINITEST_H

#include <stdio.h>
#include <string.h>

/* --- Colors (ANSI escape codes) --- */
#define MT_COLOR_RED    "\033[31m"
#define MT_COLOR_GREEN  "\033[32m"
#define MT_COLOR_YELLOW "\033[33m"
#define MT_COLOR_RESET  "\033[0m"

/* --- Test function type --- */
typedef void (*mt_test_fn)(void);
typedef void (*mt_setup_fn)(void);

/* --- Test case entry --- */
typedef struct {
    const char *name;
    mt_test_fn fn;
} mt_test_case_t;

/* --- Test suite --- */
#define MT_MAX_TESTS 128

typedef struct {
    const char *suite_name;
    mt_test_case_t tests[MT_MAX_TESTS];
    int count;
    mt_setup_fn setup;
    mt_setup_fn teardown;
    int current_failed; /* flag for current test */
} mt_suite_t;

/* --- Global state for assertion tracking --- */
extern mt_suite_t *mt_current_suite;

/* --- Macros --- */

#define MINITEST_DEFINE_SUITE(name, setup_fn, teardown_fn) \
    static mt_suite_t name = { \
        .suite_name = #name, \
        .count = 0, \
        .setup = (setup_fn), \
        .teardown = (teardown_fn), \
        .current_failed = 0 \
    }

#define MINITEST_ADD(suite, test_fn) \
    do { \
        (suite).tests[(suite).count].name = #test_fn; \
        (suite).tests[(suite).count].fn = (test_fn); \
        (suite).count++; \
    } while (0)

#define MINITEST_RUN(suite) mt_run_suite(&(suite))

/* --- Assertion macros --- */

#define MT_ASSERT(cond) do { \
    if (!(cond)) { \
        fprintf(stderr, "    %sFAIL%s: %s:%d: %s\n", \
                MT_COLOR_RED, MT_COLOR_RESET, __FILE__, __LINE__, #cond); \
        mt_current_suite->current_failed = 1; \
        return; \
    } \
} while (0)

#define MT_ASSERT_EQ_INT(expected, actual) do { \
    long _e = (long)(expected); \
    long _a = (long)(actual); \
    if (_e != _a) { \
        fprintf(stderr, "    %sFAIL%s: %s:%d: expected %ld, got %ld\n", \
                MT_COLOR_RED, MT_COLOR_RESET, __FILE__, __LINE__, _e, _a); \
        mt_current_suite->current_failed = 1; \
        return; \
    } \
} while (0)

#define MT_ASSERT_EQ_STR(expected, actual) do { \
    const char *_e = (expected); \
    const char *_a = (actual); \
    if (_e == NULL || _a == NULL || strcmp(_e, _a) != 0) { \
        fprintf(stderr, "    %sFAIL%s: %s:%d: expected \"%s\", got \"%s\"\n", \
                MT_COLOR_RED, MT_COLOR_RESET, __FILE__, __LINE__, \
                _e ? _e : "(null)", _a ? _a : "(null)"); \
        mt_current_suite->current_failed = 1; \
        return; \
    } \
} while (0)

#define MT_ASSERT_NULL(ptr) do { \
    if ((ptr) != NULL) { \
        fprintf(stderr, "    %sFAIL%s: %s:%d: expected NULL\n", \
                MT_COLOR_RED, MT_COLOR_RESET, __FILE__, __LINE__); \
        mt_current_suite->current_failed = 1; \
        return; \
    } \
} while (0)

#define MT_ASSERT_NOT_NULL(ptr) do { \
    if ((ptr) == NULL) { \
        fprintf(stderr, "    %sFAIL%s: %s:%d: expected non-NULL\n", \
                MT_COLOR_RED, MT_COLOR_RESET, __FILE__, __LINE__); \
        mt_current_suite->current_failed = 1; \
        return; \
    } \
} while (0)

/* --- Runner function --- */
int mt_run_suite(mt_suite_t *suite);

#endif /* MINITEST_H */
