# 05 - File I/O & errno

> stdio 是 C 最早的"高层抽象"。这章学会读写文件、处理错误，理解 buffered vs unbuffered。

## 前置

- [01-fundamentals](../01-fundamentals)
- [03-strings-arrays](../03-strings-arrays)

## 本章目标

- `FILE *`：`fopen` / `fclose` / `fread` / `fwrite` / `fprintf` / `fscanf` / `fgets` / `fputs`
- 文本 vs 二进制模式（`"r"` vs `"rb"`），Windows 上的 `\r\n` 翻译
- `errno` + `perror` + `strerror`（C 报错的"约定"）
- `feof` / `ferror`：循环退出条件的正确写法（**不是** `while (!feof(f))`！）
- 缓冲：`setvbuf` / `fflush`，行缓冲 vs 全缓冲
- 文件偏移：`fseek` / `ftell` / `rewind`
- 临时文件：`tmpfile` / `mkstemp`
- POSIX 低层 IO 浅尝：`open` / `read` / `write` / `close`（第 10 章细讲）

## 推荐工具

- `strace` / `dtruss` 看系统调用
- `xxd` 看二进制

## 计划要写

- `src/`: CSV 读取器、二进制结构体写入读取、wc -l 复刻
- `tests/`: 临时文件读写往返、errno 检查

## 自测

- 为啥 `while (!feof(f)) { fread(...); use(...); }` 是错的？正确写法？
- 文本模式打开二进制文件会发生什么？
- `fread` 返回值含义？怎么区分 EOF 和错误？
- `fflush(stdin)` 是 UB 还是合法？

---

**TODO**: 待补充完整代码与测试。
