#ifndef INT_LIST_H
#define INT_LIST_H

#include <stddef.h>

struct int_list_node {
    int value;
    struct int_list_node *next;
};

struct int_list {
    struct int_list_node *head;
    struct int_list_node *tail;
    size_t size;
};

void int_list_init(struct int_list *list);
int int_list_push_front(struct int_list *list, int value);
int int_list_push_back(struct int_list *list, int value);
struct int_list_node *int_list_find(struct int_list *list, int value);
int int_list_remove_first(struct int_list *list, int value);
void int_list_destroy(struct int_list *list);

#endif
