/*
 * pipeutil.c — Pipe communication between parent and child.
 */
#define _POSIX_C_SOURCE 200809L

#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include "pipeutil.h"

int pipe_child_to_parent(const char *msg, char *buf, size_t buf_size) {
    int pipefd[2]; /* [0]=read end, [1]=write end */

    if (pipe(pipefd) < 0) {
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }

    if (pid == 0) {
        /* Child: close read end, write message, exit */
        close(pipefd[0]);
        size_t len = strlen(msg);
        write(pipefd[1], msg, len);
        close(pipefd[1]);
        _exit(0);
    }

    /* Parent: close write end, read from pipe */
    close(pipefd[1]);

    ssize_t total = 0;
    ssize_t n;
    while ((size_t)total < buf_size - 1) {
        n = read(pipefd[0], buf + total, buf_size - 1 - (size_t)total);
        if (n <= 0) break;
        total += n;
    }
    buf[total] = '\0';
    close(pipefd[0]);

    /* Reap child */
    waitpid(pid, NULL, 0);

    return (int)total;
}
