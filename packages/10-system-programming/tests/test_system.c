/*
 * test_system.c — Tests for POSIX process and pipe utilities.
 */
#define _POSIX_C_SOURCE 200809L

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "process.h"
#include "pipeutil.h"

static int g_pass = 0;
static int g_total = 0;

#define RUN(name) do { ++g_total; name(); ++g_pass; printf("[PASS] %s\n", #name); } while (0)

/* --- Process tests --- */

static int child_returns_zero(void) {
    return 0;
}

static int child_returns_42(void) {
    return 42;
}

static void test_fork_run_success(void) {
    int rc = process_fork_run(child_returns_zero);
    assert(rc == 0);
}

static void test_fork_run_nonzero(void) {
    int rc = process_fork_run(child_returns_42);
    assert(rc == 42);
}

static void test_process_run_true(void) {
    /* /usr/bin/true exits with 0 */
    char *args[] = {"true", NULL};
    int rc = process_run("/usr/bin/true", args);
    assert(rc == 0);
}

static void test_process_run_false(void) {
    /* /usr/bin/false exits with 1 */
    char *args[] = {"false", NULL};
    int rc = process_run("/usr/bin/false", args);
    assert(rc == 1);
}

/* --- Pipe tests --- */

static void test_pipe_basic(void) {
    char buf[64];
    int n = pipe_child_to_parent("hello from child", buf, sizeof(buf));
    assert(n > 0);
    assert(strcmp(buf, "hello from child") == 0);
}

static void test_pipe_empty(void) {
    char buf[64];
    int n = pipe_child_to_parent("", buf, sizeof(buf));
    assert(n == 0);
    assert(buf[0] == '\0');
}

static void test_pipe_long_message(void) {
    /* Message longer than typical pipe buffer chunk */
    char msg[512];
    memset(msg, 'A', sizeof(msg) - 1);
    msg[sizeof(msg) - 1] = '\0';

    char buf[600];
    int n = pipe_child_to_parent(msg, buf, sizeof(buf));
    assert(n == 511);
    assert(strlen(buf) == 511);
}

/* --- Signal handling demo --- */

static volatile sig_atomic_t g_signal_received = 0;

static void signal_handler(int sig) {
    (void)sig;
    g_signal_received = 1;
}

static void test_signal_handling(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    assert(sigaction(SIGUSR1, &sa, NULL) == 0);

    /* Send signal to ourselves */
    g_signal_received = 0;
    kill(getpid(), SIGUSR1);
    assert(g_signal_received == 1);
}

int main(void) {
    puts("Running 10-system-programming tests...");

    RUN(test_fork_run_success);
    RUN(test_fork_run_nonzero);
    RUN(test_process_run_true);
    RUN(test_process_run_false);
    RUN(test_pipe_basic);
    RUN(test_pipe_empty);
    RUN(test_pipe_long_message);
    RUN(test_signal_handling);

    printf("%d/%d passed\n", g_pass, g_total);
    return g_pass == g_total ? 0 : 1;
}
