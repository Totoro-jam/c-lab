# 08 - Build Systems: make / CMake / Meson

> 写 hello.c 是简单的，但当你有 200 个 .c、3 种平台、5 个第三方库时，没有构建系统就是灾难。

## 前置

- [01-fundamentals](../01-fundamentals)
- [07-preprocessor-headers](../07-preprocessor-headers)

## 本章目标

- 翻译单元（translation unit）与链接
- `gcc -c` 生成 `.o`，`ar` 打包 `.a`，`-shared` 生成 `.so` / `.dylib`
- Make 基本：变量、规则、依赖、`$@` `$<` `$^`、自动依赖（`-MMD -MP`）
- Make 进阶：pattern rule、phony、parallel build（`-j`）
- CMake 基本：`add_executable` / `add_library` / `target_link_libraries`
- CMake 进阶：`find_package`、`FetchContent`、`target_compile_options`
- Meson + ninja 速览
- pkg-config 是什么、怎么用
- 静态库 vs 动态库的权衡

## 推荐工具

- `make -p`、`make -n` 看规则展开
- `cmake --build build --verbose` 看真实编译命令
- `ldd` / `otool -L` 看动态库依赖

## 计划要写

- `src/`: 一个三文件项目（`a.c`、`b.c`、`main.c`）三种构建方式各写一遍
- 横向对比：哪种工具适合什么场景

## 自测

- `gcc -c a.c b.c main.c -o foo` 和分步编译再链接，区别？
- 改了 `header.h` 为啥不重编引用它的 `.c`？怎么解决？
- 静态库链接顺序为什么重要？
- CMake 的 `target_include_directories(PRIVATE)` 和 `PUBLIC` 区别？

---

**TODO**: 待补充完整代码与示例。
