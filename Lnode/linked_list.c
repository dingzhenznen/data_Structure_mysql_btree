#include "linked_list.h"

#include <stdio.h>
#include <stdlib.h>

static ListNode *node_create(int value)
{
    ListNode *node = malloc(sizeof(*node));

    if (node == NULL) {
        return NULL;
    }

    node->data = value;
    node->next = NULL;
    return node;
}

void list_init(LinkedList *list)
{
    if (list == NULL) {
        return;
    }

    list->head = NULL;
    list->size = 0;
}

bool list_push_front(LinkedList *list, int value)
{
    ListNode *node;

    if (list == NULL) {
        return false;
    }

    node = node_create(value);
    if (node == NULL) {
        return false;
    }

    node->next = list->head;
    list->head = node;
    list->size++;
    return true;
}

bool list_push_back(LinkedList *list, int value)
{
    ListNode *node;
    ListNode *current;

    if (list == NULL) {
        return false;
    }

    node = node_create(value);
    if (node == NULL) {
        return false;
    }

    if (list->head == NULL) {
        list->head = node;
    } else {
        current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
    }

    list->size++;
    return true;
}

bool list_insert_at(LinkedList *list, size_t index, int value)
{
    ListNode *current;
    ListNode *node;

    if (list == NULL || index > list->size) {
        return false;
    }

    if (index == 0) {
        return list_push_front(list, value);
    }

    node = node_create(value);
    if (node == NULL) {
        return false;
    }

    current = list->head;
    for (size_t position = 1; position < index; position++) {
        current = current->next;
    }

    node->next = current->next;
    current->next = node;
    list->size++;
    return true;
}

bool list_find(const LinkedList *list, int value, size_t *index)
{
    const ListNode *current;
    size_t position = 0;

    if (list == NULL) {
        return false;
    }

    current = list->head;
    while (current != NULL) {
        if (current->data == value) {
            if (index != NULL) {
                *index = position;
            }
            return true;
        }
        current = current->next;
        position++;
    }

    return false;
}

bool list_delete_at(LinkedList *list, size_t index, int *deleted_value)
{
    ListNode *current;
    ListNode *removed;

    if (list == NULL || index >= list->size) {
        return false;
    }

    if (index == 0) {
        removed = list->head;
        list->head = removed->next;
    } else {
        current = list->head;
        for (size_t position = 1; position < index; position++) {
            current = current->next;
        }
        removed = current->next;
        current->next = removed->next;
    }

    if (deleted_value != NULL) {
        *deleted_value = removed->data;
    }

    free(removed);
    list->size--;
    return true;
}

void list_print(const LinkedList *list)
{
    const ListNode *current;

    if (list == NULL) {
        return;
    }

    printf("[");
    current = list->head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("] (size=%zu)\n", list->size);
}

void list_clear(LinkedList *list)
{
    ListNode *current;

    if (list == NULL) {
        return;
    }

    current = list->head;
    while (current != NULL) {
        ListNode *next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->size = 0;
}
