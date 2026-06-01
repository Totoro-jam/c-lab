# 01 - Fundamentals: C 是什么、第一个能跑能测的 C 程序

> 用最薄的工具（gcc + make + 几个 `assert`）讲清楚 C 程序的运行模型 + 项目结构 + 第一个测试。学完这章你能回答：从 .c 到可执行文件经历了哪几步、main 的签名什么含义、为什么 `int x;` 不会被初始化、自己的工程怎么用 Makefile 组织。

## 本章目标

- 理解 **预处理 → 编译 → 汇编 → 链接** 四个阶段
- 知道 main 的两种合法签名，知道 argc/argv 是什么
- 写一个有"业务函数 + 测试 main"的最小工程
- 用 Makefile 管理 `all / test / clean`
- 编译永远带 `-Wall -Wextra -Werror -std=c11`

## 如何运行

```bash
cd packages/01-fundamentals

make                          # 编译生成 build/calc 和 build/test_calc
./build/calc add 1 2          # → 3
./build/calc divide 10 2      # → 5
./build/calc even 4           # → true

make test                     # 跑测试
# Running tests...
# [PASS] add_positive
# [PASS] add_with_zero
# [PASS] divide_normal
# [PASS] divide_by_zero_returns_error
# [PASS] is_even_basic
# 5/5 passed

make clean                    # 清理
```

带 sanitizer 跑（推荐）：

```bash
make SANITIZE=1
./build/test_calc
```

## 核心概念

### 1. 从 .c 到可执行文件的四步

```
hello.c
   │
   ▼  cpp（预处理器）：展开 #include / #define / 删注释
hello.i
   │
   ▼  cc1（编译器前端）：C 源码 → 汇编
hello.s
   │
   ▼  as（汇编器）：汇编 → 目标文件
hello.o
   │
   ▼  ld（链接器）：合并多个 .o + libc + libm + ... → 可执行
hello
```

观察每一步：

```bash
gcc -E hello.c              # 只预处理，输出到 stdout
gcc -S hello.c              # 编译到汇编 hello.s
gcc -c hello.c              # 编译 + 汇编，输出 hello.o
gcc hello.c -o hello        # 完整流程
```

理解这个流程后：
- `undefined reference to 'foo'` = 链接器错误，缺 .o 或缺 lib
- `implicit declaration of function 'foo'` = 编译器错误，缺 `#include`
- `redefinition of 'foo'` = 重复定义，通常 header 缺 include guard

### 2. main 的合法签名

```c
int main(void)                                       // ✅ 不接收参数
int main(int argc, char *argv[])                     // ✅ 接收参数
int main(int argc, char **argv)                      // ✅ 等价
```

不要写 `void main()`——非标准（虽然多数编译器不报错）。

- `argc` = argument count，**总是 ≥ 1**（argv[0] 是程序名）
- `argv[argc]` = `NULL`，是个约定

### 3. 函数声明 vs 定义

```c
// header (.h)：声明 —— 告诉编译器"有这么个函数"
int add(int a, int b);

// source (.c)：定义 —— 真正的实现
int add(int a, int b) { return a + b; }
```

- 头文件里**只声明**，不定义（除非 `static inline`）
- 多个 .c 文件 #include 同一个 .h，链接时函数体只在一个 .o 里
- 重复定义 → linker error

### 4. include guard（防止头文件被多次展开）

```c
#ifndef CALC_H
#define CALC_H
/* ... 声明 ... */
#endif
```

或更短的 `#pragma once`（非标准但所有主流编译器支持）。

**为啥需要**：A 和 B 都 include 了 C；如果你的 main 又 include 了 A、B，结果 C 被展开两次 → 重复定义。

### 5. 编译选项必带

```
-std=c11               用 C11 标准（或 c17）
-Wall                  开常见警告
-Wextra                开额外警告
-Werror                警告当错处理 ← 关键，不让自己宽容
-O0 -g                 调试构建：不优化、带调试符号
-O2                    发布构建：优化
-fsanitize=address     dev 永远开
-fsanitize=undefined   dev 永远开
-MMD -MP               自动生成依赖（让 .h 改动触发重编）
```

### 6. Makefile 的最小心智模型

```makefile
target: dependencies
	command          # ← 必须用 TAB，不能用空格
```

含义：当任何 `dependencies` 比 `target` 新，就跑 `command` 把 target 生成出来。

`make` 默认跑第一个 target。`make foo` 跑 `foo` target。

### 7. 测试在 C 里怎么做

C 没有标准测试框架。最朴素的方法：写一个 `test_xxx.c`，里面 `assert(add(1,2) == 3)`，挂了就 abort。

```c
#include <assert.h>
#include "calc.h"

int main(void) {
    assert(add(1, 2) == 3);
    printf("[PASS] add\n");
    return 0;
}
```

本章用这种朴素写法。第 11 章引入 unity / cmocka。

## 代码导读

```
01-fundamentals/
├── Makefile             ← 构建：all / test / clean
├── include/
│   └── calc.h           ← 头文件（声明）
├── src/
│   ├── calc.c           ← 业务实现
│   └── main.c           ← 入口：把 calc 包装成 CLI
└── tests/
    └── test_calc.c      ← 测试（自己写 main，跑断言）
```

阅读顺序：
1. `include/calc.h` —— 看接口
2. `src/calc.c` —— 看实现
3. `src/main.c` —— 看怎么接命令行
4. `tests/test_calc.c` —— 看怎么测
5. `Makefile` —— 看怎么连起来

## 常见坑

### 坑 1：函数声明不全 = "implicit int"

```c
int main(void) {
    print_hello();        // 没 include 也没声明
    return 0;
}
```

C89 时编译器默默假设返回 `int`，C99 起报警。这是"implicit declaration"的来源。**永远 include 必要的头**。

### 坑 2：局部变量不初始化

```c
int x;
printf("%d\n", x);        // ❌ UB（可能是 0，可能是任何值）
```

修法：`int x = 0;`。

### 坑 3：scanf 读字符串没限长

```c
char buf[10];
scanf("%s", buf);         // ❌ 输入 > 10 字节就溢出
scanf("%9s", buf);        // ✅ 限长 9（留 1 字节给 \0）
```

更好：用 `fgets(buf, sizeof buf, stdin)` 然后 `strchr` 去掉换行。

### 坑 4：printf 格式串 vs 类型不匹配

```c
size_t n = 100;
printf("%d\n", n);        // ❌ size_t 在 64-bit 是 unsigned long
printf("%zu\n", n);       // ✅ z = size_t 长度修饰
```

编译加 `-Wformat=2` 能抓出来。

### 坑 5：Makefile 用空格代替 TAB

```makefile
target:
    command               # ← 这是 4 空格，会报 "missing separator"
```

必须 TAB。这是 make 最常踩的坑。

### 坑 6：忘了 `make clean` 之后改头文件没重编

如果 Makefile 没写 `-MMD` 生成依赖，`.h` 改了但 make 只检查 `.c` 时间戳，不会重编 → 用着旧 .o。

本章 Makefile 已经用 `-MMD -MP`。

## 延伸阅读

- [Modern C (Jens Gustedt)](https://gustedt.gitlabpages.inria.fr/modern-c/) — 现代 C 视角的免费书
- K&R *The C Programming Language* — 圣经
- [GCC online docs](https://gcc.gnu.org/onlinedocs/)
- [GNU Make Manual](https://www.gnu.org/software/make/manual/)
- [What Every C Programmer Should Know About Undefined Behavior (LLVM blog)](https://blog.llvm.org/2011/05/what-every-c-programmer-should-know.html)

## 自测

合上代码，回答：

1. `gcc hello.c -o hello` 隐含跑了哪 4 步？
2. `int main(void)` 和 `void main()` 哪个合法？为什么？
3. 头文件为啥需要 include guard？
4. `-Wall -Wextra -Werror` 各自什么含义？为什么 `-Werror` 重要？
5. Makefile 里 target 和 dependencies 的执行规则是？
6. 为什么 `int x;` 是 UB 但 `int x = 0;` 不是？

下一章：`02-pointers-memory` —— C 的灵魂与地狱。
