# 11 - Testing with Unity / cmocka

> 第 01 章用 `assert` 写测试，能跑但难用。这章引入真正的测试框架。

## 前置

- [01-fundamentals](../01-fundamentals)
- [08-build-systems](../08-build-systems)

## 本章目标

- 为啥需要测试框架（test 列举、selective run、好的失败信息、setup/teardown）
- Unity：极简，单文件，嵌入式友好
  - `TEST_ASSERT_EQUAL_INT`、`TEST_ASSERT_EQUAL_STRING`、`TEST_ASSERT_NULL` 等
  - `setUp` / `tearDown`
  - `RUN_TEST(...)` + `UNITY_BEGIN/END`
- cmocka：支持 mock、参数化、grouped tests
  - `cmocka_unit_test_setup_teardown`
  - `will_return` / `expect_value` / `check_expected`
- Check：fork 隔离每个 case（崩了不带塌全场）
- 覆盖率：`gcc --coverage` + `gcov` / `lcov`
- CI 集成：Makefile target、退出码约定

## 推荐工具

- Unity（github.com/ThrowTheSwitch/Unity）
- cmocka
- gcov + lcov

## 计划要写

- `src/`: 复用 06-data-structures 的 `vec` / `hashmap`
- `tests/`: 用 Unity 重写 01 章的 calc 测试做对比；用 cmocka 演示 mock
- 覆盖率报告生成脚本

## 自测

- Unity 的 `TEST_ASSERT_EQUAL_INT(a, b)` 比 `assert(a == b)` 强在哪？
- 如果一个测试调用了 `abort()`，会不会带塌后续测试？
- 怎么 mock 一个第三方库函数（不动其源码）？
- 覆盖率 100% 等于代码没 bug 吗？

---

**TODO**: 待补充完整代码与测试。
