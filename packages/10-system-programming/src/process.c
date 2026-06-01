/*
 * process.c — Process management using fork/exec/wait.
 */
#define _POSIX_C_SOURCE 200809L

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#include "process.h"

int process_run(const char *cmd, char *const args[]) {
    pid_t pid = fork();
    if (pid < 0) {
        return -1; /* fork failed */
    }

    if (pid == 0) {
        /* Child process */
        execv(cmd, args);
        _exit(127); /* execv only returns on error */
    }

    /* Parent process */
    int status;
    if (waitpid(pid, &status, 0) < 0) {
        return -1;
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    return -1; /* abnormal termination */
}

int process_fork_run(int (*fn)(void)) {
    pid_t pid = fork();
    if (pid < 0) {
        return -1;
    }

    if (pid == 0) {
        /* Child */
        int rc = fn();
        _exit(rc);
    }

    /* Parent */
    int status;
    if (waitpid(pid, &status, 0) < 0) {
        return -1;
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    return -1;
}
