#include <stdio.h>
#include <stdlib.h>

struct LNode {
      int data;
      struct LNode *next;
};

int main(void)
{
      struct LNode *head, *middle, *last;
      head = malloc(sizeof(struct LNode));
      middle = malloc(sizeof(struct LNode));
      last = malloc(sizeof(struct LNode));

      if (head == NULL || middle == NULL || last == NULL) {
          free(head);
          free(middle);
          free(last);
          return 1;
      }

      head->data = 1;
      head->next = middle;

      middle->data = 2;
      middle->next = last;

      last->data = 3;
      last->next = NULL;

      for (struct LNode *current = head; current != NULL; current = current->next) {
          printf("%d\n", current->data);
      }

      free(head);
      free(middle);
      free(last);

      return 0;
}