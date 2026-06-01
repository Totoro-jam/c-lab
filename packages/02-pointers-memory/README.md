# 02 - Pointers & Memory: C 的灵魂与地狱

> C 真正的"分水岭"。这章过了，后面一马平川；没过，后面每个 bug 都来自这里。

## 前置

- [01-fundamentals](../01-fundamentals)

## 本章目标

- 理解"指针就是地址 + 类型"
- `*` / `&` / `[]` / `->` 四个操作的语义
- 栈 vs 堆，函数调用栈帧
- `malloc` / `calloc` / `realloc` / `free` 全套
- 指针算术：`p + 1` 加的是 `sizeof(*p)` 字节
- void* 是怎么实现"泛型"的
- const 修饰位置的三种含义（`const int *`、`int * const`、`int const * const`）
- 函数指针

## 推荐工具

- 必带：`-fsanitize=address,undefined`
- 加分：`valgrind --tool=memcheck`（Linux）/ `leaks`（macOS）

## 计划要写

- `src/`: 几个真实 bug 演示文件（leak、UAF、double-free、越界）—— 故意写错让 ASan 抓
- `tests/`: 一个简单 `Vec<T>` 用 void* 实现，覆盖 push/grow/free
- README 核心：栈帧示意图 + ASan 输出怎么读

## 自测

- `int *p` 和 `int* p` 有区别吗？
- `const int *p` 和 `int * const p` 含义分别是？
- 为啥不能 `free` 栈上变量？
- `p + 1` 加几个字节，谁决定？

---

**TODO**: 待补充完整代码与测试。
