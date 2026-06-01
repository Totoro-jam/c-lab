/*
 * variant.h — Tagged union (variant type) implementation.
 *
 * Demonstrates:
 *  - Struct with enum tag + union payload
 *  - Opaque struct pattern (forward declaration)
 *  - Bit fields
 *  - Memory alignment concepts
 */
#ifndef VARIANT_H
#define VARIANT_H

#include <stddef.h>
#include <stdbool.h>

/* --- Tagged Union (Variant) --- */

typedef enum {
    VAR_INT,
    VAR_DOUBLE,
    VAR_STRING,
    VAR_BOOL
} var_kind_t;

typedef struct {
    var_kind_t kind;
    union {
        long i;
        double d;
        char *s;   /* heap-allocated, owned by the variant */
        bool b;
    } value;
} variant_t;

/* Create variants of each type */
variant_t variant_int(long val);
variant_t variant_double(double val);
variant_t variant_string(const char *val); /* copies the string */
variant_t variant_bool(bool val);

/* Get the kind tag */
var_kind_t variant_kind(const variant_t *v);

/* Type-safe accessors. Return 0 on success, -1 on type mismatch. */
int variant_get_int(const variant_t *v, long *out);
int variant_get_double(const variant_t *v, double *out);
int variant_get_string(const variant_t *v, const char **out);
int variant_get_bool(const variant_t *v, bool *out);

/* Free resources (only needed for string variants) */
void variant_destroy(variant_t *v);

/* --- Bit Fields Demo --- */

typedef struct {
    unsigned int is_active  : 1;
    unsigned int priority   : 3; /* 0-7 */
    unsigned int category   : 4; /* 0-15 */
    unsigned int reserved   : 24;
} flags_t;

/* --- Opaque struct pattern --- */

/* Forward declaration only — internal layout hidden in .c file */
typedef struct opaque_handle opaque_handle_t;

opaque_handle_t *opaque_create(int x, int y);
int opaque_get_x(const opaque_handle_t *h);
int opaque_get_y(const opaque_handle_t *h);
void opaque_destroy(opaque_handle_t *h);

#endif /* VARIANT_H */
