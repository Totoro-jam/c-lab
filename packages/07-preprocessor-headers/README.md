# 07 - Preprocessor & Header Hygiene

> `#include` 是简单的"文本插入"。理解它你就懂为什么 C 编译慢、为啥头文件那么讲究。

## 前置

- [01-fundamentals](../01-fundamentals)

## 本章目标

- 预处理阶段做了什么（`gcc -E` 看输出）
- `#include` 的两种形式（`<>` vs `""`）与查找路径
- include guard：`#ifndef X / #define X / #endif` vs `#pragma once`
- 宏：对象宏 / 函数宏 / 可变参数宏 `__VA_ARGS__`
- 宏的坑：求值次数、操作符优先级、`do { ... } while (0)` 包裹
- 条件编译：`#if` / `#ifdef` / `#elif` / 平台分发
- 字符串化 `#x` 与连接 `##`（X-macro 套路）
- 头文件分层：public header vs private header
- 前向声明减少依赖

## 推荐工具

- `gcc -E` / `clang -E` 看预处理结果
- `cpplint` / `iwyu` (include-what-you-use)

## 计划要写

- `src/`: 一组演示宏陷阱的例子（`#define SQUARE(x) x*x` 配 `SQUARE(1+2)`）
- `src/`: X-macro 自动生成 enum + name 表
- `tests/`: 编译期断言 `_Static_assert`

## 自测

- `#pragma once` 和 include guard 哪个更可靠？为啥？
- `#define MAX(a,b) ((a)>(b)?(a):(b))` 和 `MAX(i++, j++)` 一起用会出什么事？
- 头文件里能不能定义函数？`static inline` 又是怎么回事？
- 前向声明能省什么、不能省什么？

---

**TODO**: 待补充完整代码与测试。
