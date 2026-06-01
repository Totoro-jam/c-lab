# C 工具链 / 库横向对比

## 1. 编译器

| | gcc | clang | msvc | tcc |
|---|---|---|---|---|
| 平台 | 所有 Unix | macOS 默认、所有 Unix | Windows | 教学 / JIT |
| 警告 | 多 | 多 + 错误信息好 | 中 | 少 |
| Sanitizer | ASan/UBSan/TSan/MSan | 同左（更全） | ASan | ❌ |
| 编译速度 | 中 | 快 | 中 | **极快** |
| LTO/PGO | ✅ | ✅ | ✅ | ❌ |
| C++ 支持 | ✅ | ✅ | ✅ | ❌ |
| 推荐 | Linux | macOS / 跨平台 | Windows | 嵌入式脚本 |

**结论**：clang 错误信息更友好（新手用 clang，老 Linux 项目用 gcc）。

---

## 2. 构建系统

| | make | cmake | meson | bazel | ninja |
|---|---|---|---|---|---|
| 类型 | 构建工具 | 构建生成器 | 构建生成器 | 构建工具 | 构建工具（快） |
| 写法 | Makefile（手写规则） | CMakeLists.txt（高层 DSL） | meson.build（python-ish） | BUILD（python-ish） | build.ninja（机器生成） |
| 依赖管理 | 手写 | find_package | wrap | repository | 不管 |
| 跨平台 | 弱（Unix） | ✅ | ✅ | ✅ | ✅（生成自他） |
| 学习曲线 | 低 | 中 | 中 | 高 | 低 |
| 推荐 | 教学/小项目 | **绝大多数 C/C++ 项目** | 现代项目 | Google 风 monorepo | 配合 cmake |

**结论**：教学用 make，生产用 cmake（行业事实标准）。

---

## 3. 测试框架

| | Unity | cmocka | Criterion | check | Google Test |
|---|---|---|---|---|---|
| 语言 | 纯 C | 纯 C | 纯 C | 纯 C | C++（**测 C 也可**） |
| Mock | ❌ | ✅ | ❌ | ❌ | gmock |
| 隔离（fork per test） | ❌ | ❌ | ✅ | ✅ | ❌ |
| 嵌入式 | ✅ 极轻量 | 中 | 中 | 中 | 重 |
| 推荐 | 嵌入式 / 教学 | 通用 + mock | 隔离要求高 | 老 Linux | 同时有 C++ 项目 |

**结论**：从 cmocka 入手，需要 mock 时再加。

---

## 4. JSON 库

| | cJSON | jansson | json-c | nlohmann | RapidJSON |
|---|---|---|---|---|---|
| 语言 | C | C | C | **C++** | **C++** |
| 速度 | 中 | 中 | 中 | 中 | **极快** |
| API | 简单 | 中 | 中 | 极美 | 复杂 |
| 依赖 | 0 | 0 | 0 | header-only | header-only |
| 推荐 | C 默认 | C 备选 | C 备选 | C++ 默认 | C++ 性能极致 |

---

## 5. HTTP 客户端

| | libcurl | wget（lib 版） | nng | tinyhttp |
|---|---|---|---|---|
| 协议 | 极全（HTTP/2/3、FTP、SMTP、IMAP、...) | HTTP/FTP | NNG 自家 | HTTP |
| 复杂度 | 高 | 中 | 中 | 极低 |
| 推荐 | **生产** | 不推荐 | 替代消息库 | 极简内嵌 |

**结论**：libcurl 是事实标准。`curl --libcurl` 能从命令直接生成 C 代码模板。

---

## 6. 嵌入式数据库

| | SQLite | LevelDB | RocksDB | LMDB |
|---|---|---|---|---|
| 类型 | SQL | KV | KV | KV |
| 速度 | 中 | 快 | **极快** | 极快读 |
| 大小 | 小 | 中 | 大 | 极小 |
| 事务 | ACID | 有限 | 完整 | ACID |
| 推荐 | **绝大多数** | KV 简单 | 高写 | 嵌入只读 |

**结论**：99% 选 SQLite。

---

## 7. 日志库

| | 自己写 printf | log.c (rxi) | zlog | log4c |
|---|---|---|---|---|
| 大小 | 0 | 单文件 | 中 | 大 |
| 级别 | 自己加 | ✅ | ✅ | ✅ |
| 多线程 | 自己处理 | ✅ | ✅ | ✅ |
| 推荐 | 小项目 | 中小项目 | 高级需求 | ❌ |

**结论**：[log.c](https://github.com/rxi/log.c) 单文件 200 行，绝大多数项目够用。

---

## 8. libc 实现

| | glibc | musl | uclibc | newlib |
|---|---|---|---|---|
| 平台 | Linux 主流 | Alpine / 静态链 | 嵌入式 | bare-metal |
| 大小 | 大 | **小** | 小 | 小 |
| 速度 | 中 | 中 | 中 | 取决场景 |
| 兼容性 | 完整 | **大多数兼容** | 部分 | 部分 |
| 推荐 | 桌面/服务器 | Alpine 镜像 / 静态二进制 | OpenWrt | 单片机 |

---

## 9. 真实项目栈推荐

**通用应用**：
```
cmake + clang + cmocka + cJSON + libcurl + SQLite
+ -Wall -Wextra -Werror -fsanitize=address,undefined（dev）
```

**嵌入式**：
```
make + gcc-arm + Unity + 自己写极简 stack
```

**高性能服务**：
```
cmake + clang/gcc + io_uring/epoll + 自定义 arena + jemalloc
```
