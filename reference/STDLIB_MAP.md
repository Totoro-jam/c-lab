# C 标准库速查（C11/C17）

| 头文件 | 主要功能 |
|---|---|
| `<stdio.h>` | `printf` / `scanf` / `fopen` / `fread` / `getline` |
| `<stdlib.h>` | `malloc` / `free` / `atoi` / `qsort` / `exit` |
| `<string.h>` | `strlen` / `strcmp` / `strncpy` / `memcpy` / `memset` |
| `<ctype.h>` | `isdigit` / `isalpha` / `tolower` |
| `<math.h>` | `sin` / `sqrt` / `pow` / `fabs`（链接加 `-lm`） |
| `<time.h>` | `time` / `clock` / `strftime` / `localtime` |
| `<errno.h>` | `errno` / `EINVAL` / `ENOMEM` |
| `<assert.h>` | `assert(cond)`（`NDEBUG` 时去掉） |
| `<stdint.h>` | `uint8_t` / `int32_t` / `intptr_t` / `INT32_MAX` |
| `<stdbool.h>` | `bool` / `true` / `false`（C99+） |
| `<stddef.h>` | `size_t` / `ptrdiff_t` / `NULL` / `offsetof` |
| `<limits.h>` | `INT_MAX` / `CHAR_BIT` |
| `<float.h>` | `DBL_EPSILON` / `FLT_MAX` |
| `<stdarg.h>` | `va_list` 变长参数 |
| `<setjmp.h>` | `setjmp/longjmp`（异常模拟，少用） |
| `<signal.h>` | `signal` / `raise` |
| `<threads.h>` | C11 线程（很少实现） |
| `<stdatomic.h>` | C11 原子 |

## POSIX 扩展（非 C 标准，但 Unix 都有）

| 头文件 | 功能 |
|---|---|
| `<unistd.h>` | `read` / `write` / `fork` / `exec` / `pipe` / `getpid` |
| `<fcntl.h>` | `open` / `O_RDONLY` |
| `<sys/stat.h>` | `stat` / `mkdir` |
| `<sys/types.h>` | `pid_t` / `off_t` |
| `<sys/wait.h>` | `wait` / `waitpid` |
| `<sys/socket.h>` | socket / bind / listen |
| `<netinet/in.h>` | `sockaddr_in` |
| `<arpa/inet.h>` | `htons` / `inet_pton` |
| `<dirent.h>` | `opendir` / `readdir` |
| `<dlfcn.h>` | `dlopen` / `dlsym`（动态加载） |
| `<pthread.h>` | POSIX 线程 |

## 高频常用 API

```c
// 字符串
size_t strlen(const char *s);
char  *strchr(const char *s, int c);
char  *strstr(const char *haystack, const char *needle);
int    strcmp(const char *a, const char *b);
char  *strncpy(char *dst, const char *src, size_t n);
char  *strdup(const char *s);                  // POSIX
int    snprintf(char *buf, size_t n, const char *fmt, ...);

// 内存
void  *memcpy(void *dst, const void *src, size_t n);
void  *memmove(void *dst, const void *src, size_t n);     // 重叠安全
void  *memset(void *dst, int c, size_t n);
int    memcmp(const void *a, const void *b, size_t n);

// 分配
void  *malloc(size_t n);
void  *calloc(size_t cnt, size_t size);        // 自动清零
void  *realloc(void *p, size_t n);
void   free(void *p);

// IO
FILE  *fopen(const char *path, const char *mode);
int    fclose(FILE *fp);
size_t fread (void *buf, size_t sz, size_t n, FILE *fp);
size_t fwrite(const void *buf, size_t sz, size_t n, FILE *fp);
char  *fgets(char *buf, int n, FILE *fp);      // ✅ 替代 gets
int    fprintf(FILE *fp, const char *fmt, ...);
ssize_t getline(char **lineptr, size_t *n, FILE *fp);    // POSIX，自动 alloc

// 排序 / 搜索
void  qsort (void *base, size_t n, size_t size, int (*cmp)(const void *, const void *));
void *bsearch(const void *key, const void *base, size_t n, size_t size, int (*cmp)(...));

// 数字转换
long   strtol (const char *s, char **endptr, int base);   // ✅ 替代 atoi（atoi 不报错）
double strtod(const char *s, char **endptr);
```

## 禁用 API（不安全，永远不要新写）

| 不要用 | 用什么替代 |
|---|---|
| `gets()` | `fgets()` |
| `strcpy()` | `snprintf()` / `strncpy + 终止符` |
| `strcat()` | `snprintf()` |
| `sprintf()` | `snprintf()` |
| `atoi()` | `strtol()` |
| `scanf("%s", ...)` | `fgets()` + `sscanf` |
