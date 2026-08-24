#include "linked_list.h"

#include <stdio.h>

int main(void)
{
    LinkedList list;
    size_t index;
    int deleted_value;

    list_init(&list);

    list_push_back(&list, 10);
    list_push_back(&list, 20);
    list_push_front(&list, 5);
    list_insert_at(&list, 2, 15);
    list_print(&list);

    if (list_find(&list, 20, &index)) {
        printf("value 20 is at index %zu\n", index);
    }

    if (list_delete_at(&list, 1, &deleted_value)) {
        printf("deleted value: %d\n", deleted_value);
    }
    list_print(&list);

    list_clear(&list);
    list_print(&list);
    return 0;
}
