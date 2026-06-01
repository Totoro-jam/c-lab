#ifndef HTTP_DEMO_H
#define HTTP_DEMO_H

#include <stddef.h>

typedef struct {
    char  *data;
    size_t len;
    size_t cap;
} http_response_t;

void http_response_init(http_response_t *resp);
void http_response_free(http_response_t *resp);

int http_get(const char *url, http_response_t *resp);

#endif
