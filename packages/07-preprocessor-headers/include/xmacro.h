/*
 * xmacro.h — X-Macro pattern demonstration.
 *
 * X-macros auto-generate enum values AND string name tables
 * from a single definition. Eliminates enum-name synchronization bugs.
 */
#ifndef XMACRO_H
#define XMACRO_H

/* The X-macro list: define all colors in one place */
#define COLOR_LIST \
    X(RED)         \
    X(GREEN)       \
    X(BLUE)        \
    X(YELLOW)      \
    X(CYAN)        \
    X(MAGENTA)

/* Generate the enum */
typedef enum {
#define X(name) COLOR_##name,
    COLOR_LIST
#undef X
    COLOR_COUNT  /* sentinel: total number of colors */
} color_t;

/*
 * Get the string name of a color enum value.
 * Returns "UNKNOWN" for out-of-range values.
 */
const char *color_name(color_t c);

/*
 * Parse a color name string into enum value.
 * Returns -1 if not found.
 */
int color_parse(const char *name, color_t *out);

#endif /* XMACRO_H */
