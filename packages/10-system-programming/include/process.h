/*
 * process.h — Process management utilities (POSIX).
 *
 * Demonstrates fork/exec/wait patterns.
 */
#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

/*
 * Run a command and return its exit status.
 * cmd: full path to executable
 * args: NULL-terminated argument list (args[0] = program name)
 * Returns exit status (0-255), or -1 on error.
 */
int process_run(const char *cmd, char *const args[]);

/*
 * Fork a child that runs the given function, wait for it.
 * Returns the child's exit status, or -1 on error.
 */
int process_fork_run(int (*fn)(void));

#endif /* PROCESS_H */
