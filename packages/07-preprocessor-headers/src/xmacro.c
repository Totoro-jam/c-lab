/*
 * xmacro.c — X-Macro implementation.
 *
 * The X-macro pattern generates both enum and name table
 * from the same list, keeping them always in sync.
 */
#include <string.h>
#include "xmacro.h"

/* Generate the name table using the same X-macro list */
static const char *color_names[] = {
#define X(name) #name,
    COLOR_LIST
#undef X
};

const char *color_name(color_t c) {
    if (c < 0 || c >= COLOR_COUNT) {
        return "UNKNOWN";
    }
    return color_names[c];
}

int color_parse(const char *name, color_t *out) {
    for (int i = 0; i < COLOR_COUNT; i++) {
        if (strcmp(color_names[i], name) == 0) {
            *out = (color_t)i;
            return 0;
        }
    }
    return -1;
}
