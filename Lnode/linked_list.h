#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct ListNode {
    int data;
    struct ListNode *next;
} ListNode;

typedef struct {
    ListNode *head;
    size_t size;
} LinkedList;

void list_init(LinkedList *list);
bool list_push_front(LinkedList *list, int value);
bool list_push_back(LinkedList *list, int value);
bool list_insert_at(LinkedList *list, size_t index, int value);
bool list_find(const LinkedList *list, int value, size_t *index);
bool list_delete_at(LinkedList *list, size_t index, int *deleted_value);
void list_print(const LinkedList *list);
void list_clear(LinkedList *list);

#endif
