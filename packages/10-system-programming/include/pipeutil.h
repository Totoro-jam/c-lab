/*
 * pipeutil.h — Pipe utilities (POSIX).
 *
 * Demonstrates pipe() + fork() for parent-child communication.
 */
#ifndef PIPEUTIL_H
#define PIPEUTIL_H

#include <stddef.h>

/*
 * Create a pipe, fork a child that writes msg into it.
 * Parent reads the message into buf (up to buf_size - 1 bytes).
 * Returns number of bytes read, or -1 on error.
 */
int pipe_child_to_parent(const char *msg, char *buf, size_t buf_size);

#endif /* PIPEUTIL_H */
