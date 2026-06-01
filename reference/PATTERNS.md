# C 工程模式

C 没有 class / 异常 / 模板。这些模式是社区"约定"出来填空白的。

---

## 1. 错误处理：返回 int，0 成功，非 0 错误码

```c
int read_file(const char *path, char **out_buf, size_t *out_n) {
    FILE *fp = fopen(path, "rb");
    if (!fp) return -errno;       // 返回负 errno 让调用方区分
    // ...
    *out_buf = buf;
    *out_n   = n;
    return 0;
}

// 调用方
char *buf; size_t n;
int rc = read_file("x", &buf, &n);
if (rc != 0) {
    fprintf(stderr, "read_file: %s\n", strerror(-rc));
    return -1;
}
```

**约定**：值返回结果，错误通过 return code。**不要把结果和错误混在一个值里**（Go 风格 `(val, err)` 在 C 里靠出参实现）。

---

## 2. goto cleanup（C 里的 RAII 替代）

```c
int do_work(const char *path) {
    int rc = -1;
    FILE *fp = NULL;
    char *buf = NULL;

    fp = fopen(path, "r");
    if (!fp) goto done;

    buf = malloc(1024);
    if (!buf) goto done;

    if (fread(buf, 1, 1024, fp) == 0) goto done;

    /* ... */
    rc = 0;

done:
    free(buf);
    if (fp) fclose(fp);
    return rc;
}
```

这是 Linux kernel 的标准模式。**`goto cleanup` 是 C 里少数被赞同的 goto 用法**。

---

## 3. opaque 结构体（信息隐藏）

```c
// foo.h —— 头文件只暴露类型名，不暴露字段
typedef struct foo Foo;
Foo *foo_create(void);
void foo_destroy(Foo *f);
int  foo_do_stuff(Foo *f, int x);
```

```c
// foo.c —— 内部才知道结构
struct foo {
    int x;
    char *name;
};
Foo *foo_create(void) { ... }
```

**调用方不能直接 `foo.x`**，全靠函数。等价于 OO 的封装。

---

## 4. 句柄 (handle)

```c
typedef struct file_handle *FileHandle;     // 不透明指针
FileHandle file_open(const char *path);
int        file_read(FileHandle h, void *buf, size_t n);
void       file_close(FileHandle h);
```

`FileHandle` 是个 typedef，用户拿到的就是个指针，但用户不知道结构。SQLite / curl / OpenSSL 都这么干。

---

## 5. 函数指针表（vtable / 多态）

```c
typedef struct {
    int (*open)(void *self, const char *path);
    int (*read)(void *self, void *buf, size_t n);
    int (*close)(void *self);
} Stream;

// 内存流实现
static int mem_open(void *self, const char *p) { ... }
static int mem_read(void *self, void *buf, size_t n) { ... }
static Stream MEM_STREAM = { mem_open, mem_read, ... };

// 网络流实现
static Stream NET_STREAM = { net_open, net_read, ... };

// 调用
Stream *s = &MEM_STREAM;
s->open(s, "x");
s->read(s, buf, 100);
```

C 实现"虚函数"的标准做法。Linux 文件系统、内核驱动都是这个模式。

---

## 6. 回调

```c
typedef int (*foreach_cb)(void *item, void *ctx);

int list_foreach(List *l, foreach_cb cb, void *ctx) {
    for (Node *n = l->head; n; n = n->next) {
        int rc = cb(n->data, ctx);
        if (rc != 0) return rc;
    }
    return 0;
}

// 调用方
int print_one(void *item, void *ctx) { printf("%s\n", (char*)item); return 0; }
list_foreach(list, print_one, NULL);
```

**`void *ctx` 是 C 的 closure 替代**——调用方塞自己的状态进去。

---

## 7. 资源所有权约定（注释驱动）

```c
/**
 * Returns newly-allocated string. Caller must free().
 */
char *make_name(int id);

/**
 * Borrowed reference. Function does not free or store @p name.
 */
void greet(const char *name);

/**
 * Takes ownership of @p name. Function will free() it.
 * Caller must not free after this call.
 */
void greet_take(char *name);

/**
 * Returns borrowed pointer into @p ctx. Valid only as long as @p ctx is.
 */
const char *get_user_name(const Context *ctx);
```

C 没有 Rust 的所有权检查，**只有靠注释 + code review + 一致命名风格**。一个项目把约定写成 CONTRIBUTING.md。

---

## 8. PIMPL（pointer to implementation）

```c
// widget.h
typedef struct widget Widget;
typedef struct widget_impl WidgetImpl;
struct widget { WidgetImpl *impl; };

// widget.c
struct widget_impl { /* 真正字段 */ };
```

**好处**：改 impl 不需要重编译所有 include 头文件的人。大型 C/C++ 项目用得多。

---

## 9. flexible array member（C99+）

```c
typedef struct {
    size_t len;
    char data[];           // 必须是最后一个字段
} String;

String *make(const char *s) {
    size_t n = strlen(s);
    String *str = malloc(sizeof *str + n + 1);
    str->len = n;
    memcpy(str->data, s, n + 1);
    return str;
}
```

**好处**：把 header 和 payload 合在一次 malloc，**省一次内存分配 + cache 友好**。Redis sds 就是这套。

---

## 10. arena / pool 分配器

```c
typedef struct { char *base; size_t used, cap; } Arena;

void *arena_alloc(Arena *a, size_t n) {
    if (a->used + n > a->cap) return NULL;
    void *p = a->base + a->used;
    a->used += n;
    return p;
}

void arena_reset(Arena *a) { a->used = 0; }      // 一次性释放全部
```

**适合**：一批短命对象（一个请求处理周期内的所有分配）。`arena_reset` 比 `free` 100 个对象快得多，也避免 leak。

---

## 11. 错误码 → 消息

```c
typedef enum { E_OK, E_NOMEM, E_INVAL, E_IO, E_MAX } ErrCode;

static const char *err_msg[] = {
    [E_OK]    = "ok",
    [E_NOMEM] = "out of memory",
    [E_INVAL] = "invalid argument",
    [E_IO]    = "io error",
};

const char *err_str(ErrCode e) {
    return (e >= 0 && e < E_MAX) ? err_msg[e] : "unknown";
}
```

**designated initializer** (`[E_OK] = ...`) 让你按 enum 名字初始化数组，加新错误码不会错位。

---

## 12. 头文件设计 + include guard

```c
// foo.h
#ifndef FOO_H
#define FOO_H

/* ... 声明 ... */

#endif /* FOO_H */
```

或 `#pragma once`（非标准但所有主流编译器都支持）。
