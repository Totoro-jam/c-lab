# 03 - Strings & Arrays: C 字符串的全部陷阱

> "C 字符串"= char 数组 + 末尾 \0 字节。这个朴素定义带来了一打安全漏洞。学完知道何时用啥、永远不要用啥。

## 前置

- [02-pointers-memory](../02-pointers-memory)

## 本章目标

- C 字符串 = `char[] / char *` + `'\0'` 终止符
- `<string.h>` 全套：strlen / strcmp / strcpy / strncpy / strdup / strchr / strstr / strtok
- **永远禁用**：`gets`、`strcpy`、`strcat`、`sprintf`
- **永远首选**：`fgets`、`snprintf`、`strncpy + 终止符`、`strndup`
- 字符串字面量 vs `char[]`（一个只读、一个可写）
- 多维数组 vs 数组指针 vs 指针数组（`int a[3][4]` vs `int (*p)[4]` vs `int *p[3]`）
- `argv` 解析

## 推荐工具

- ASan + UBSan
- `-Wformat=2`、`-Wstringop-overflow=4`

## 计划要写

- `src/`: 安全字符串拷贝、token 分割（不污染原串）、CSV 行解析
- `tests/`: 边界（空串、超长、含 `\0`）

## 自测

- `char *s = "hello"; s[0] = 'H';` 会发生什么？
- `strncpy` 不会自动加 `\0`？为啥？
- `snprintf` 返回值什么含义？
- 二维数组 `a[i][j]` 编译器怎么算地址？

---

**TODO**: 待补充完整代码与测试。
