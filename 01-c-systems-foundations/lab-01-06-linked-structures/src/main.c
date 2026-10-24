#include "int_list.h"

#include <stdio.h>

int main(void)
{
    struct int_list list;

    int_list_init(&list);

    (void)int_list_push_back(&list, 10);
    (void)int_list_push_back(&list, 20);
    (void)int_list_push_front(&list, 5);
    (void)int_list_push_back(&list, 30);
    (void)int_list_remove_first(&list, 20);

    printf("size=%zu values:", list.size);

    for (struct int_list_node *node = list.head;
         node != NULL;
         node = node->next) {
        printf(" %d", node->value);
    }
    printf("\n");

    int_list_destroy(&list);
    return 0;
}
