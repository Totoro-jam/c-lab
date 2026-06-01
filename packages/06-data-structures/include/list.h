/*
 * list.h — Singly linked list with void* data.
 */
#ifndef DS_LIST_H
#define DS_LIST_H

#include <stddef.h>

typedef struct ds_list_node {
    void *data;
    struct ds_list_node *next;
} ds_list_node_t;

typedef struct {
    ds_list_node_t *head;
    size_t len;
} ds_list_t;

void ds_list_init(ds_list_t *list);
int ds_list_push_front(ds_list_t *list, void *data);
int ds_list_push_back(ds_list_t *list, void *data);
void *ds_list_pop_front(ds_list_t *list);
void *ds_list_get(const ds_list_t *list, size_t index);
size_t ds_list_len(const ds_list_t *list);
void ds_list_free(ds_list_t *list);

#endif /* DS_LIST_H */
