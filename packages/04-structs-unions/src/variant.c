/*
 * variant.c — Tagged union and opaque struct implementations.
 */
#include <stdlib.h>
#include <string.h>

#include "variant.h"

/* --- Tagged Union --- */

variant_t variant_int(long val) {
    variant_t v;
    v.kind = VAR_INT;
    v.value.i = val;
    return v;
}

variant_t variant_double(double val) {
    variant_t v;
    v.kind = VAR_DOUBLE;
    v.value.d = val;
    return v;
}

variant_t variant_string(const char *val) {
    variant_t v;
    v.kind = VAR_STRING;
    v.value.s = strdup(val);
    return v;
}

variant_t variant_bool(bool val) {
    variant_t v;
    v.kind = VAR_BOOL;
    v.value.b = val;
    return v;
}

var_kind_t variant_kind(const variant_t *v) {
    return v->kind;
}

int variant_get_int(const variant_t *v, long *out) {
    if (v->kind != VAR_INT) {
        return -1;
    }
    *out = v->value.i;
    return 0;
}

int variant_get_double(const variant_t *v, double *out) {
    if (v->kind != VAR_DOUBLE) {
        return -1;
    }
    *out = v->value.d;
    return 0;
}

int variant_get_string(const variant_t *v, const char **out) {
    if (v->kind != VAR_STRING) {
        return -1;
    }
    *out = v->value.s;
    return 0;
}

int variant_get_bool(const variant_t *v, bool *out) {
    if (v->kind != VAR_BOOL) {
        return -1;
    }
    *out = v->value.b;
    return 0;
}

void variant_destroy(variant_t *v) {
    if (v->kind == VAR_STRING) {
        free(v->value.s);
        v->value.s = NULL;
    }
}

/* --- Opaque Handle --- */

struct opaque_handle {
    int x;
    int y;
    /* internal fields hidden from users */
    int _internal_state;
};

opaque_handle_t *opaque_create(int x, int y) {
    opaque_handle_t *h = malloc(sizeof(opaque_handle_t));
    if (h == NULL) {
        return NULL;
    }
    h->x = x;
    h->y = y;
    h->_internal_state = 0;
    return h;
}

int opaque_get_x(const opaque_handle_t *h) {
    return h->x;
}

int opaque_get_y(const opaque_handle_t *h) {
    return h->y;
}

void opaque_destroy(opaque_handle_t *h) {
    free(h);
}
