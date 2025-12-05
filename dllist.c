#include <stdio.h>
#include <stdlib.h>
#include "dllist.h"

void dlist_init(DList *list) {
    list->head = list->tail = NULL;
    list->size = 0;
}

static Node* create_node(int value) {
    Node *n = (Node*)malloc(sizeof(Node));
    if (!n) return NULL;
    n->data = value;
    n->prev = n->next = NULL;
    return n;
}

void dlist_clear(DList *list) {
    Node *cur = list->head;
    while (cur) {
        Node *next = cur->next;
        free(cur);
        cur = next;
    }
    list->head = list->tail = NULL;
    list->size = 0;
}

void dlist_push_front(DList *list, int value) {
    Node *n = create_node(value);
    if (!n) return;
    n->next = list->head;
    if (list->head) list->head->prev = n;
    list->head = n;
    if (!list->tail) list->tail = n;
    list->size++;
}

void dlist_push_back(DList *list, int value) {
    Node *n = create_node(value);
    if (!n) return;
    n->prev = list->tail;
    if (list->tail) list->tail->next = n;
    list->tail = n;
    if (!list->head) list->head = n;
    list->size++;
}

bool dlist_insert_after(DList *list, int after_value, int value) {
    Node *pos = dlist_find(list, after_value);
    if (!pos) return false;
    Node *n = create_node(value);
    if (!n) return false;
    n->next = pos->next;
    n->prev = pos;
    pos->next = n;
    if (n->next) n->next->prev = n;
    else list->tail = n;
    list->size++;
    return true;
}

bool dlist_remove(DList *list, int value) {
    Node *n = dlist_find(list, value);
    if (!n) return false;
    if (n->prev) n->prev->next = n->next;
    else list->head = n->next;
    if (n->next) n->next->prev = n->prev;
    else list->tail = n->prev;
    free(n);
    list->size--;
    return true;
}

Node* dlist_find(DList *list, int value) {
    Node *cur = list->head;
    while (cur) {
        if (cur->data == value) return cur;
        cur = cur->next;
    }
    return NULL;
}

void dlist_print_forward(DList *list) {
    printf("Forward (size=%d): ", list->size);
    Node *cur = list->head;
    while (cur) {
        printf("%d", cur->data);
        if (cur->next) printf(" -> ");
        cur = cur->next;
    }
    printf("\n");
}

void dlist_print_backward(DList *list) {
    printf("Backward (size=%d): ", list->size);
    Node *cur = list->tail;
    while (cur) {
        printf("%d", cur->data);
        if (cur->prev) printf(" -> ");
        cur = cur->prev;
    }
    printf("\n");
}
