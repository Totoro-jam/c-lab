# 06 - Data Structures: 自己造轮子

> C 没有 STL。这章亲手实现链表、栈、队列、哈希表、动态数组——理解所有"高级"语言背后的代价。

## 前置

- [02-pointers-memory](../02-pointers-memory)
- [04-structs-unions](../04-structs-unions)

## 本章目标

- 单链表 / 双链表：插入、删除、遍历、释放
- 动态数组 `vec_t`：grow 策略（2x vs 1.5x）、amortized O(1) push
- 栈 / 队列（基于数组或链表）
- 哈希表：开放寻址 vs 拉链法、装载因子、再哈希
- 二叉搜索树 / 简易堆
- 泛型怎么做：`void *` + 用户提供 `cmp` / `free` 函数指针
- 侵入式数据结构（intrusive list，Linux kernel 风格）

## 推荐工具

- ASan / valgrind 测内存
- `gprof` 看性能热点

## 计划要写

- `src/`: `vec.[ch]`、`list.[ch]`、`hashmap.[ch]`
- `tests/`: 大量随机插入/删除、内存全部释放校验
- 文档：每种结构的 Big-O 表与实现取舍

## 自测

- 为什么 vector 容量翻倍而不是 +10？
- 哈希表装载因子超过多少应该 rehash？
- 侵入式链表相比"普通"链表的优势？
- 怎么写一个"传任意类型都行"的链表？

---

**TODO**: 待补充完整代码与测试。
