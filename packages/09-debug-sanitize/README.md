# 09 - Debugging & Sanitizers

> C 把内存安全的责任甩给了你。这章学会工具链——让 bug 自己跳出来。

## 前置

- [02-pointers-memory](../02-pointers-memory)

## 本章目标

- `gdb` / `lldb` 基本：`b` `r` `n` `s` `c` `bt` `p` `info locals`
- 调试符号 `-g`、`-O0` vs `-O2` 下断点能不能命中
- `printf` 调试的最后倔强：`fprintf(stderr, ...)`、`__FILE__` / `__LINE__`
- AddressSanitizer (ASan)：堆栈越界、UAF、double-free、leak
- UndefinedBehaviorSanitizer (UBSan)：signed overflow、null deref、misalignment
- ThreadSanitizer (TSan)：data race
- MemorySanitizer (MSan)：未初始化读
- valgrind（Linux）：memcheck、cachegrind、callgrind
- `assert` / `_Static_assert` 双面盾
- core dump 捕获与 `bt full`

## 推荐工具

- 编译开关：`-g -O1 -fno-omit-frame-pointer -fsanitize=address,undefined`
- macOS：`leaks --atExit`
- Linux：`valgrind`、`rr` 时间旅行调试

## 计划要写

- `src/`: 故意写 5 种典型 bug（leak、use-after-free、stack overflow、UB int overflow、race）
- 每个 bug：原始版本 + sanitizer 报告截屏 + 修复版本
- README 核心：怎么读 ASan 输出

## 自测

- 为啥 ASan 编译比正常慢 2x、内存涨 2x？
- ASan 报告里的 "freed by thread T0 here" 怎么定位代码？
- `assert` 在 release 模式还在吗？怎么强制保留？
- gdb 里 `p *p` 报 "Cannot access memory" 通常意味着什么？

---

**TODO**: 待补充完整代码与示例。
