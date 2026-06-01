# C 内存管理军规

> 见到下面任何一种问题，都是潜在的崩溃 / 安全漏洞。每条都背下来。

---

## 1. 每个 `malloc` 必须配一个 `free`

```c
char *buf = malloc(n);
if (!buf) return -1;       // ✅ 检查 NULL
// ... 用 buf
free(buf);
buf = NULL;                // ✅ 防止 use-after-free
```

**最重要的一句**：**谁分配，谁释放**——除非你在 API 文档里明确说"返回值需要调用方 free"。

---

## 2. `malloc` 之后立刻检 NULL

```c
char *p = malloc(BIG);
if (p == NULL) {           // ❌ 漏掉这步 = 段错误的根源
    return -1;
}
```

`malloc` 失败返回 `NULL`，不是 errno，不抛错。

---

## 3. free 之后立刻把指针置 NULL

```c
free(p);
p = NULL;                  // ✅ 之后再 free(p) 是合法 no-op，但 *p 立刻段错误
```

否则 double-free / use-after-free 会出现"有时崩有时不崩"——最难调试的 bug。

---

## 4. 数组下标永远检查边界

```c
char buf[64];
size_t i = user_input();
if (i >= 64) return -1;    // ✅
buf[i] = 'x';
```

C 不会越界检查。越界写 = 栈溢出 = 远程代码执行（实战中的真实攻击面）。

---

## 5. 字符串永远预留 `\0` 一字节

```c
char buf[64];
strncpy(buf, src, 64);     // ❌ 如果 src ≥ 64 字节，没有终止符 → 读到天荒地老
strncpy(buf, src, 63);
buf[63] = '\0';            // ✅
// 或
snprintf(buf, sizeof buf, "%s", src);   // ✅ 永远会加 \0
```

**首选 `snprintf`，几乎没有理由用 `strcpy/strcat/sprintf`**。

---

## 6. `sizeof` 数组 vs 指针的陷阱

```c
char a[10];
char *p = a;
sizeof(a);                 // 10  ✅
sizeof(p);                 // 8 (64-bit)  ← 指针的大小，不是数组！

void f(char arr[]) {
    sizeof(arr);           // 8！arr 在函数参数里退化成指针了
}
```

函数参数里别用 `sizeof`，传 size 进去。

---

## 7. 结构体动态分配的"复制即引用"

```c
typedef struct { char *name; int age; } User;
User u1 = {strdup("Alice"), 30};
User u2 = u1;              // ⚠️ name 指针被复制，两个 user 指同一块内存
free(u1.name);
// u2.name 现在是 dangling pointer
```

要"深拷贝"必须显式 `strdup` / `memcpy`。

---

## 8. 返回栈上指针 = 灾难

```c
char *bad(void) {
    char buf[100];
    return buf;            // ❌ 函数返回后栈帧销毁，buf 失效
}
```

要返回字符串：
- 用 `malloc` 分配并由调用方 `free`
- 让调用方传 buffer 进来：`void f(char *out, size_t n)`
- 用 `static` 局部数组（**仅单线程、不可重入**）

---

## 9. realloc 失败时不要覆盖原指针

```c
p = realloc(p, n);         // ❌ 失败时返回 NULL，p 原地址泄漏
```

修法：

```c
void *q = realloc(p, n);
if (!q) { /* p 仍然有效 */ return -1; }
p = q;
```

---

## 10. malloc(0) 行为不确定

```c
void *p = malloc(0);       // 可能返回 NULL，可能返回唯一指针，平台决定
```

修法：自己判断 `if (n == 0) return NULL;`。

---

## 11. 计算大小用 `sizeof(*ptr)` 不要硬编码类型

```c
int *p = malloc(n * sizeof(int));        // ⚠️ 如果 p 改成 long*，这里没改 → bug
int *p = malloc(n * sizeof(*p));         // ✅ 跟着 p 类型走，永远对
```

---

## 12. 字节顺序不要假设

```c
uint32_t x = 0x12345678;
char *p = (char *)&x;
p[0];   // 取决于小端/大端，跨平台代码用 htonl/ntohl
```

---

## 13. 永远初始化局部变量

```c
int x;
if (cond) x = 1;
return x;                  // ❌ cond 假时 x 未初始化 → UB
```

修法：声明时给默认值 `int x = 0;`。

---

## 14. 用工具兜底

| 工具 | 抓什么 |
|---|---|
| `-Wall -Wextra -Werror` | 编译期警告 |
| `-fsanitize=address` (ASan) | 越界、UAF、leak |
| `-fsanitize=undefined` (UBSan) | 整数溢出、除零、空指针 deref |
| `valgrind --leak-check=full` | leak、未初始化读 |
| `clang-tidy` | 静态分析 |

**至少 dev 环境永远开 ASan + UBSan**：

```sh
gcc -O1 -g -fsanitize=address,undefined main.c -o main
./main
```

---

## 15. 所有权约定（项目里写在 doc 注释）

```c
/**
 * Return a newly-allocated string. Caller must free().
 */
char *make_greeting(const char *name);

/**
 * Caller retains ownership of @p name. Function does not store it.
 */
void greet(const char *name);

/**
 * Takes ownership of @p name. Function will free() it.
 */
void greet_take(char *name);
```

C 没有 Rust 的所有权检查，**靠注释 + code review + 一致命名**。
