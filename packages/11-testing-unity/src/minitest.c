/*
 * minitest.c — Test framework runner implementation.
 */
#include "minitest.h"

/* Global pointer to current suite (used by assertion macros) */
mt_suite_t *mt_current_suite = NULL;

int mt_run_suite(mt_suite_t *suite) {
    mt_current_suite = suite;

    int passed = 0;
    int failed = 0;

    printf("\n%s=== Suite: %s (%d tests) ===%s\n",
           MT_COLOR_YELLOW, suite->suite_name, suite->count, MT_COLOR_RESET);

    for (int i = 0; i < suite->count; i++) {
        suite->current_failed = 0;

        /* setUp */
        if (suite->setup) {
            suite->setup();
        }

        /* Run test */
        suite->tests[i].fn();

        /* tearDown */
        if (suite->teardown) {
            suite->teardown();
        }

        if (suite->current_failed) {
            printf("  %s[FAIL]%s %s\n",
                   MT_COLOR_RED, MT_COLOR_RESET, suite->tests[i].name);
            failed++;
        } else {
            printf("  %s[PASS]%s %s\n",
                   MT_COLOR_GREEN, MT_COLOR_RESET, suite->tests[i].name);
            passed++;
        }
    }

    printf("\n%sResults: %d passed, %d failed, %d total%s\n",
           failed > 0 ? MT_COLOR_RED : MT_COLOR_GREEN,
           passed, failed, passed + failed,
           MT_COLOR_RESET);

    mt_current_suite = NULL;
    return failed == 0 ? 0 : 1;
}
