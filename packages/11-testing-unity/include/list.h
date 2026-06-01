/*
 * list.h — Singly linked list (copied from ch06 for testing).
 */
#ifndef TEST_LIST_H
#define TEST_LIST_H

#include <stddef.h>

typedef struct tlist_node {
    void *data;
    struct tlist_node *next;
} tlist_node_t;

typedef struct {
    tlist_node_t *head;
    size_t len;
} tlist_t;

void tlist_init(tlist_t *list);
int tlist_push_front(tlist_t *list, void *data);
int tlist_push_back(tlist_t *list, void *data);
void *tlist_pop_front(tlist_t *list);
size_t tlist_len(const tlist_t *list);
void tlist_free(tlist_t *list);

#endif /* TEST_LIST_H */
