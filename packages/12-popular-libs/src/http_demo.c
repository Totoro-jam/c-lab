#include "http_demo.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

void http_response_init(http_response_t *resp)
{
    resp->data = NULL;
    resp->len  = 0;
    resp->cap  = 0;
}

void http_response_free(http_response_t *resp)
{
    free(resp->data);
    resp->data = NULL;
    resp->len  = 0;
    resp->cap  = 0;
}

static size_t write_cb(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    http_response_t *resp = userdata;
    size_t bytes = size * nmemb;
    size_t needed = resp->len + bytes + 1;

    if (needed > resp->cap) {
        size_t newcap = (resp->cap == 0) ? 4096 : resp->cap;
        while (newcap < needed) newcap *= 2;
        char *tmp = realloc(resp->data, newcap);
        if (!tmp) return 0;
        resp->data = tmp;
        resp->cap  = newcap;
    }

    memcpy(resp->data + resp->len, ptr, bytes);
    resp->len += bytes;
    resp->data[resp->len] = '\0';
    return bytes;
}

int http_get(const char *url, http_response_t *resp)
{
    CURL *curl = curl_easy_init();
    if (!curl) return -1;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode cc = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return (cc == CURLE_OK) ? 0 : (int)cc;
}
