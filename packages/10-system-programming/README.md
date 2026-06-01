# 10 - System Programming (POSIX)

> 走出 stdio，直接和操作系统对话。文件描述符、进程、信号、IPC、套接字——所有上层抽象的地基。

## 前置

- [05-file-io](../05-file-io)

## 本章目标

- 文件描述符 vs `FILE *`
- POSIX 低层 IO：`open` / `read` / `write` / `close` / `lseek` / `dup` / `dup2`
- `fcntl`：非阻塞、O_APPEND、文件锁
- 进程：`fork` / `exec` 家族 / `wait` / `waitpid` / 僵尸与孤儿
- 信号：`signal` vs `sigaction`（强烈优先 `sigaction`）、可重入函数
- 管道：`pipe` + `fork` 经典套路
- IPC 速览：共享内存（shm_open）、消息队列、信号量
- 套接字 sockets：`socket` / `bind` / `listen` / `accept` / `connect` / `send` / `recv`
- I/O 多路复用：`select` / `poll` / `epoll`(Linux) / `kqueue`(macOS)
- 线程：pthreads（`pthread_create` / `join` / `mutex` / `cond`）

## 推荐工具

- `strace` / `dtruss`
- `lsof`
- `netcat` 调试 socket
- man 手册（`man 2 read`、`man 7 signal`）

## 计划要写

- `src/`: tiny shell（fork + exec + 管道）
- `src/`: TCP echo server（poll 版）
- `tests/`: 进程退出码、信号处理

## 自测

- `fork()` 之后父子进程哪些资源是共享的、哪些是 copy-on-write？
- 为啥 `printf` 在信号处理函数里是 unsafe 的？
- 为啥要 `sigaction` 而不是 `signal`？
- TCP `accept` 阻塞时 Ctrl-C，会发生什么？

---

**TODO**: 待补充完整代码与示例。
