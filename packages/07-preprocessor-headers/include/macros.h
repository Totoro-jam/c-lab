/*
 * macros.h — Demonstration of various macro patterns.
 *
 * Demonstrates:
 *  - Object macros vs function macros
 *  - Proper parenthesization
 *  - do { ... } while(0) wrapping
 *  - Variadic macros (__VA_ARGS__)
 *  - Stringification (#) and token pasting (##)
 *  - Conditional compilation
 */
#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>

/* --- Object macros --- */
#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 3

/* --- Function-like macros with proper parens --- */

/* WRONG: #define SQUARE(x) x*x  -- SQUARE(1+2) gives 1+2*1+2 = 5 */
/* CORRECT: fully parenthesized */
#define SQUARE(x) ((x) * (x))

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define CLAMP(val, lo, hi) (MAX((lo), MIN((val), (hi))))

/* Array length (compile-time only for real arrays, NOT pointers) */
#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))

/* --- do { ... } while(0) pattern for multi-statement macros --- */
#define SWAP_INT(a, b) do { \
    int _tmp = (a);         \
    (a) = (b);              \
    (b) = _tmp;             \
} while (0)

/* --- Variadic macros --- */
#define LOG_DEBUG(fmt, ...) \
    fprintf(stderr, "[DEBUG] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

/* --- Stringification --- */
#define STRINGIFY(x) #x
#define XSTRINGIFY(x) STRINGIFY(x)  /* expand x first, then stringify */

/* --- Token pasting --- */
#define CONCAT(a, b) a##b

/* --- Conditional compilation --- */
#ifdef DEBUG_MODE
#define DBG_PRINT(msg) fprintf(stderr, "[DBG] %s\n", (msg))
#else
#define DBG_PRINT(msg) ((void)0)
#endif

/* --- Compile-time assert (C11 _Static_assert) --- */
#define STATIC_ASSERT(cond, msg) _Static_assert((cond), msg)

#endif /* MACROS_H */
