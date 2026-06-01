# C 常见未定义行为（UB）清单

> UB 不一定崩。可能"看起来对的"。但编译器可以基于"UB 不会发生"做激进优化，于是同一份代码在 -O0 和 -O2 下行为完全不同。**UB 是 C 最危险的概念，比段错误还危险**。

---

## 1. 整数溢出（有符号）

```c
int x = INT_MAX;
x + 1;                     // ❌ UB（无符号溢出是 well-defined 模 2^n）
```

修法：用无符号或检测：

```c
if (x > INT_MAX - 1) /* overflow */;
```

---

## 2. 空指针解引用 / 算术

```c
int *p = NULL;
*p;                        // ❌ UB
p + 1;                     // ❌ UB（即便不解引用）
```

---

## 3. 越界访问

```c
int a[10];
a[10];                     // ❌ UB（不只是错值——可能让编译器删掉后续代码）
```

---

## 4. use-after-free / double-free

```c
free(p);
*p = 1;                    // ❌
free(p);                   // ❌
```

---

## 5. 未初始化变量读

```c
int x;
printf("%d", x);           // ❌
```

---

## 6. 严格别名（strict aliasing）

```c
int x = 0x12345;
float f = *(float *)&x;    // ❌ 通过不兼容类型解引用 → UB
```

修法：用 `memcpy`：

```c
float f;
memcpy(&f, &x, sizeof f);  // ✅
```

或 `union`（C99 起合法，C++ 不合法）。

---

## 7. 修改字面字符串

```c
char *s = "hello";
s[0] = 'H';                // ❌ 字面量在只读段
```

修法：`char s[] = "hello";`（数组拷贝在栈上）。

---

## 8. 同一表达式中多次修改同一变量

```c
i = i++ + ++i;             // ❌ UB（顺序点之间多次修改）
a[i] = i++;                // ❌
```

---

## 9. 移位超出位宽

```c
uint32_t x = 1;
x << 32;                   // ❌ UB（移位 >= 类型位宽）
x << -1;                   // ❌
```

---

## 10. 除零

```c
int z = 0;
int q = 1 / z;             // ❌
```

---

## 11. 调用 main 返回后用 atexit 注册函数访问 main 的局部

栈帧已销毁。

---

## 12. 不带 return 的 non-void 函数

```c
int f(void) {
    if (cond) return 1;
    // 没 return → UB（不是返回 0）
}
```

---

## 13. printf 格式串和参数不匹配

```c
printf("%d", 1.5);         // ❌
printf("%s", 42);          // ❌
```

修法：编译加 `-Wformat=2`。

---

## 14. 比较不同对象的指针

```c
int a, b;
&a < &b;                   // ❌ 不同对象间的关系比较 UB（==/!= 合法）
```

---

## 15. 修改 const

```c
const int x = 5;
int *p = (int *)&x;
*p = 6;                    // ❌
```

---

## 16. 越过对象边界的指针算术

```c
int a[10];
int *p = a + 11;           // ❌（a + 10 合法但不能 deref）
int *q = a - 1;            // ❌
```

---

## 17. 多线程数据竞争（未同步的读写）

```c
// 线程 1
g_counter++;

// 线程 2
printf("%d", g_counter);   // ❌ UB（C11 之后明确）
```

修法：`_Atomic int g_counter;` 或加锁。

---

## 18. 通过函数指针调用类型不匹配的函数

```c
void f(int);
void (*p)(double) = (void (*)(double))f;
p(1.0);                    // ❌
```

---

## 19. 在 sequence point 之间读和修改同一对象

```c
a[i++] = i;                // ❌ 不清楚 i 是被修改前还是后
```

---

## 20. 关闭/释放后再使用 stream / fd

```c
fclose(fp);
fprintf(fp, ...);          // ❌
```

---

## 检测 UB 的工具

```sh
# UBSan：运行时抓
gcc -fsanitize=undefined -g main.c
./a.out
# 输出：runtime error: signed integer overflow: 2147483647 + 1 ...

# ASan：内存类 UB
gcc -fsanitize=address -g main.c

# clang-tidy / cppcheck：静态分析
```

**dev 环境永远开 -fsanitize=address,undefined。**
