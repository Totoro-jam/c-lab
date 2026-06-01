/*
 * list.c — Singly linked list implementation.
 */
#include <stdlib.h>
#include "list.h"

void ds_list_init(ds_list_t *list) {
    list->head = NULL;
    list->len = 0;
}

int ds_list_push_front(ds_list_t *list, void *data) {
    ds_list_node_t *node = malloc(sizeof(ds_list_node_t));
    if (node == NULL) {
        return -1;
    }
    node->data = data;
    node->next = list->head;
    list->head = node;
    list->len++;
    return 0;
}

int ds_list_push_back(ds_list_t *list, void *data) {
    ds_list_node_t *node = malloc(sizeof(ds_list_node_t));
    if (node == NULL) {
        return -1;
    }
    node->data = data;
    node->next = NULL;

    if (list->head == NULL) {
        list->head = node;
    } else {
        ds_list_node_t *cur = list->head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = node;
    }
    list->len++;
    return 0;
}

void *ds_list_pop_front(ds_list_t *list) {
    if (list->head == NULL) {
        return NULL;
    }
    ds_list_node_t *node = list->head;
    void *data = node->data;
    list->head = node->next;
    free(node);
    list->len--;
    return data;
}

void *ds_list_get(const ds_list_t *list, size_t index) {
    ds_list_node_t *cur = list->head;
    for (size_t i = 0; i < index && cur != NULL; i++) {
        cur = cur->next;
    }
    return cur != NULL ? cur->data : NULL;
}

size_t ds_list_len(const ds_list_t *list) {
    return list->len;
}

void ds_list_free(ds_list_t *list) {
    ds_list_node_t *cur = list->head;
    while (cur != NULL) {
        ds_list_node_t *next = cur->next;
        free(cur);
        cur = next;
    }
    list->head = NULL;
    list->len = 0;
}
