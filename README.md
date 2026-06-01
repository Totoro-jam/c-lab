# c-lab

C 语言通识实验场（C99 / C11 / C17）。学完后能看懂主流开源 C 项目（Linux kernel 模块、SQLite、curl、Redis）的代码风格、能正确管理内存、能用 gdb/valgrind/sanitizer 排错。

## 这个仓库不是什么

- 不是"30 天学会 C"——C 简单但坑深，速成必出事
- 不是 C++ 教程（C++ 在隔壁 `cpp-lab/`）
- 不替代 K&R 圣经，是 K&R 的"现代工程化补充"

## 这个仓库是什么

- **可跑代码 + 解释 why 的双轨**。每章一个独立目录，自带 `Makefile`，`make && make test` 直接跑
- **围绕实战必须**：指针、内存、字符串、结构体、文件 IO、构建、调试、系统编程、测试、主流库（SQLite / libcurl / cJSON）
- **不回避 UB（未定义行为）**：每个坑都解释"为什么不能这么写"

## 学习路径

| 章节 | 主题 | 工具 / 库 | 难度 |
|---|---|---|---|
| [01-fundamentals](packages/01-fundamentals) | 语法、编译、main、stdio | gcc, clang | ⭐ |
| [02-pointers-memory](packages/02-pointers-memory) | 指针、地址、malloc/free、内存模型 | stdlib | ⭐⭐⭐ |
| [03-strings-arrays](packages/03-strings-arrays) | C 字符串陷阱、`<string.h>`、缓冲区安全 | stdlib | ⭐⭐ |
| [04-structs-unions](packages/04-structs-unions) | struct / union / enum / typedef / 对齐 | stdlib | ⭐⭐ |
| [05-file-io](packages/05-file-io) | stdio、文件、errno、二进制 IO | stdlib | ⭐⭐ |
| [06-data-structures](packages/06-data-structures) | 手写链表、栈、队列、哈希表 | stdlib | ⭐⭐⭐ |
| [07-preprocessor-headers](packages/07-preprocessor-headers) | `#include`、macro、header guard、宏陷阱 | cpp（预处理器） | ⭐⭐ |
| [08-build-systems](packages/08-build-systems) | Makefile、依赖、增量、CMake 入门 | make, cmake | ⭐⭐⭐ |
| [09-debug-sanitize](packages/09-debug-sanitize) | gdb、lldb、valgrind、asan/ubsan/tsan | gdb, valgrind | ⭐⭐⭐ |
| [10-system-programming](packages/10-system-programming) | POSIX、fork/exec、signal、pthread、socket | POSIX | ⭐⭐⭐⭐ |
| [11-testing-unity](packages/11-testing-unity) | unity / cmocka / criterion 单测 | unity, cmocka | ⭐⭐⭐ |
| [12-popular-libs](packages/12-popular-libs) | SQLite C API、libcurl、cJSON | sqlite3, curl, cjson | ⭐⭐⭐ |

强烈建议**按顺序学**。第 02 章（指针/内存）是分水岭，没过这关后面都吃力。

## 速查参考（随用随翻）

- [reference/MEMORY_RULES.md](reference/MEMORY_RULES.md) — 内存管理"军规"
- [reference/UB_LIST.md](reference/UB_LIST.md) — 常见未定义行为清单
- [reference/STDLIB_MAP.md](reference/STDLIB_MAP.md) — C 标准库按用途速查
- [reference/PATTERNS.md](reference/PATTERNS.md) — 常用工程模式（错误处理、所有权约定、句柄、回调）
- [reference/COMPARISON.md](reference/COMPARISON.md) — gcc vs clang、make vs cmake、test frameworks、libc 实现

## 前置工具

```bash
# macOS
xcode-select --install              # 装 clang + make
brew install gdb valgrind cmake     # 调试 + 构建（valgrind 在 macOS 上有限制，arm64 用 leaks 替代）

# Linux (Ubuntu/Debian)
sudo apt install build-essential gdb valgrind cmake clang
```

## 怎么用

### 第一次

```bash
cd packages/01-fundamentals
make            # 编译
make test       # 跑测试
make clean      # 清理
```

每章都有 `Makefile`，按上面三个目标统一。

### 一次跑全部章节

```bash
# 在仓库根目录
make            # 编译全部章节
make test       # 跑全部测试（失败章节会报告，不中断其余章节）
make clean      # 清理全部编译产物
```

### 日常学习

每章 README 都有：
1. **本章目标**
2. **如何运行**
3. **核心概念**（why）
4. **代码导读**
5. **常见坑**（C 的"坑"比其他语言密度高 10 倍，重点看）
6. **延伸阅读**

## 学习心法

- **永远开 sanitizer**：编译加 `-fsanitize=address,undefined`，能抓 90% 的内存 bug
- **每写 `malloc` 立刻写 `free`**——肌肉记忆，别拖
- **不要 `gets()` / `strcpy()` / `sprintf()`**——直接禁用，用 `fgets`/`strncpy`/`snprintf`
- **编译加 `-Wall -Wextra -Werror`**——警告必须当错处理
- **看 K&R + 看 Linux kernel coding style**

## 给维护者（将来的你）

- 添新章节请放在 `packages/NN-name/`
- 每章必须有 `Makefile` 提供 `all / test / clean` 三个目标
- 每章必须能独立编译，**不依赖根目录**
