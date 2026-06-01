# 12 - Popular C Libraries 速览

> 现实里没人从 0 写所有东西。这章用两个最常见的 C 库（SQLite、libcurl）写一个完整的 CRUD + HTTP demo，体会"C 调库"的手感。

## 前置

- 前面所有章
- macOS: `sqlite3` 和 `libcurl` 系统自带
- Linux: `sudo apt install libsqlite3-dev libcurl4-openssl-dev`

## 本章目标

1. 用 SQLite C API 完成增删查改（prepared statement、bind、step）
2. 用 libcurl 发 HTTP GET 请求并收集响应
3. 理解"C 调第三方库"的通用模式：header → link → pkg-config

## 文件结构

```
12-popular-libs/
├── include/
│   ├── db_demo.h        # SQLite 封装接口
│   └── http_demo.h      # libcurl 封装接口
├── src/
│   ├── db_demo.c        # SQLite CRUD 实现
│   └── http_demo.c      # HTTP GET 实现
├── tests/
│   └── test_popular_libs.c  # 9 个测试用例
└── Makefile
```

## 如何运行

```bash
make            # 编译
make test       # 跑测试（SQLite 用 :memory:，libcurl 访问 httpbin.org）
make clean      # 清理

# 如果没有网络，跳过 HTTP 测试：
SKIP_NETWORK=1 make test
```

## 核心概念

### SQLite — "嵌入式数据库"

不需要启动数据库进程，`#include <sqlite3.h>` + 链接 `-lsqlite3` 就能用。

关键 API 调用链：

```
sqlite3_open → sqlite3_prepare_v2 → sqlite3_bind_* → sqlite3_step → sqlite3_finalize → sqlite3_close
```

**为什么用 prepared statement 而不是 `sqlite3_exec` 拼字符串？** 防 SQL 注入，性能也更好。

### libcurl — "HTTP 全家桶"

`#include <curl/curl.h>` + 链接 `-lcurl`。

关键模式：

```
curl_easy_init → curl_easy_setopt(多次) → curl_easy_perform → curl_easy_cleanup
```

收响应体需要提供一个 **write callback**——curl 每收到一块数据就调一次，你负责拼到 buffer 里。

## 代码导读

| 文件 | 看什么 |
|---|---|
| `db_demo.c` | prepared statement 完整流程、bind 参数、错误处理 |
| `http_demo.c` | write callback 模式、动态 buffer 扩容（和 chapter 06 的 vec 异曲同工） |
| `test_popular_libs.c` | SQLite 用 `:memory:` 做测试——无残留、无竞争 |

## 常见坑

- **忘了 `sqlite3_finalize`**：每个 `prepare` 必须配一个 `finalize`，否则内存泄漏
- **`sqlite3_step` 返回值**：成功插入返回 `SQLITE_DONE`，查询有行返回 `SQLITE_ROW`，别搞混
- **curl write callback 返回值**：必须返回实际处理的字节数，返回 0 curl 会当作错误中断
- **忘了 `curl_easy_cleanup`**：和 `malloc/free` 一样，init 了就要 cleanup

## C 库选型速查

| 用途 | 推荐 | 备注 |
|---|---|---|
| 嵌入式 SQL | **SQLite** | 单文件、零配置、生产级 |
| HTTP 请求 | **libcurl** | 支持几乎所有协议 |
| JSON 解析 | **cJSON** | 单文件、API 简单 |
| JSON（高性能） | **yyjson** | zero-copy，比 cJSON 快 10 倍 |
| 异步 IO | **libuv** | Node.js 底层 |
| TLS | **mbedTLS** / **OpenSSL** | 嵌入式选 mbedTLS |
| 哈希表 | **uthash** | 单 header，纯宏 |
| 单文件工具库 | **stb_*.h** | 图片、字体、噪声等 |
| 测试框架 | **Unity** / **cmocka** | 见第 11 章 |

## 延伸阅读

- [SQLite C/C++ Interface](https://www.sqlite.org/c3ref/intro.html)
- [libcurl easy interface](https://curl.se/libcurl/c/)
- [cJSON GitHub](https://github.com/DaveGamble/cJSON)
- [stb single-file libraries](https://github.com/nothings/stb)
