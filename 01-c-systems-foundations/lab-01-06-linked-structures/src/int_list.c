#include "int_list.h"

#include <stdlib.h>

void int_list_init(struct int_list *list)
{
    list->head = NULL;
    list->tail = NULL;
    list->size = 0U;
}

static struct int_list_node *make_node(int value)
{
    struct int_list_node *node = malloc(sizeof *node);

    if (node == NULL) {
        return NULL;
    }

    node->value = value;
    node->next = NULL;
    return node;
}

int int_list_push_front(struct int_list *list, int value)
{
    struct int_list_node *node;

    if (list == NULL) {
        return 0;
    }

    node = make_node(value);
    if (node == NULL) {
        return 0;
    }

    node->next = list->head;
    list->head = node;

    if (list->tail == NULL) {
        list->tail = node;
    }

    ++list->size;
    return 1;
}

int int_list_push_back(struct int_list *list, int value)
{
    struct int_list_node *node;

    if (list == NULL) {
        return 0;
    }

    node = make_node(value);
    if (node == NULL) {
        return 0;
    }

    if (list->tail != NULL) {
        list->tail->next = node;
    } else {
        list->head = node;
    }

    list->tail = node;
    ++list->size;
    return 1;
}

struct int_list_node *int_list_find(struct int_list *list, int value)
{
    if (list == NULL) {
        return NULL;
    }

    for (struct int_list_node *node = list->head;
         node != NULL;
         node = node->next) {
        if (node->value == value) {
            return node;
        }
    }

    return NULL;
}

int int_list_remove_first(struct int_list *list, int value)
{
    struct int_list_node *previous = NULL;
    struct int_list_node *current;

    if (list == NULL) {
        return 0;
    }

    current = list->head;

    while (current != NULL) {
        if (current->value == value) {
            if (previous == NULL) {
                list->head = current->next;
            } else {
                previous->next = current->next;
            }

            if (list->tail == current) {
                list->tail = previous;
            }

            free(current);
            --list->size;

            if (list->size == 0U) {
                list->head = NULL;
                list->tail = NULL;
            }

            return 1;
        }

        previous = current;
        current = current->next;
    }

    return 0;
}

void int_list_destroy(struct int_list *list)
{
    struct int_list_node *current;

    if (list == NULL) {
        return;
    }

    current = list->head;

    while (current != NULL) {
        struct int_list_node *next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0U;
}
