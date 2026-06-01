/*
 * list.c — Singly linked list (testing subject).
 */
#include <stdlib.h>
#include "list.h"

void tlist_init(tlist_t *list) {
    list->head = NULL;
    list->len = 0;
}

int tlist_push_front(tlist_t *list, void *data) {
    tlist_node_t *node = malloc(sizeof(tlist_node_t));
    if (node == NULL) return -1;
    node->data = data;
    node->next = list->head;
    list->head = node;
    list->len++;
    return 0;
}

int tlist_push_back(tlist_t *list, void *data) {
    tlist_node_t *node = malloc(sizeof(tlist_node_t));
    if (node == NULL) return -1;
    node->data = data;
    node->next = NULL;
    if (list->head == NULL) {
        list->head = node;
    } else {
        tlist_node_t *cur = list->head;
        while (cur->next != NULL) cur = cur->next;
        cur->next = node;
    }
    list->len++;
    return 0;
}

void *tlist_pop_front(tlist_t *list) {
    if (list->head == NULL) return NULL;
    tlist_node_t *node = list->head;
    void *data = node->data;
    list->head = node->next;
    free(node);
    list->len--;
    return data;
}

size_t tlist_len(const tlist_t *list) {
    return list->len;
}

void tlist_free(tlist_t *list) {
    tlist_node_t *cur = list->head;
    while (cur != NULL) {
        tlist_node_t *next = cur->next;
        free(cur);
        cur = next;
    }
    list->head = NULL;
    list->len = 0;
}
