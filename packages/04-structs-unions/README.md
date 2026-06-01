# 04 - Structs, Unions, Enums, Typedef

> C 的"复合类型"。学完能定义干净的领域模型、理解内存对齐、用 enum + switch 写出清晰的状态机。

## 前置

- [02-pointers-memory](../02-pointers-memory)

## 本章目标

- `struct` 声明、初始化（含 C99 designated init `(struct Point){.x=1,.y=2}`）
- 成员访问：`.` vs `->`（点 vs 箭头）
- 内存对齐与 padding（`offsetof`、`alignof`、`#pragma pack`）
- `union`：节省内存的"多选一"，及其类型安全风险
- `enum`：常量集合，配 switch
- `typedef`：起别名（隐藏指针 / opaque handle）
- bit field：紧凑标志位

## 推荐工具

- `pahole` 看结构体布局（Linux）
- `__builtin_offsetof` / `<stddef.h>` 的 `offsetof`

## 计划要写

- `src/`: 简单 token 结构（演示 tagged union 模式：`enum kind` + `union value`）
- `tests/`: offsetof 验算、struct copy 语义

## 自测

- 为什么 `struct { char c; int i; }` 大小是 8 而不是 5？
- `struct A *` 和 `A *` 哪个能用？区别？
- union 怎么"安全"使用？为什么叫 tagged union？

---

**TODO**: 待补充完整代码与测试。
