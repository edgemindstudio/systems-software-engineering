#include "int_list.h"

#include <assert.h>

int main(void)
{
    struct int_list list;

    int_list_init(&list);
    assert(list.size == 0U);
    assert(list.head == NULL);
    assert(list.tail == NULL);

    assert(int_list_push_back(&list, 2) != 0);
    assert(int_list_push_front(&list, 1) != 0);
    assert(int_list_push_back(&list, 3) != 0);

    assert(list.size == 3U);
    assert(list.head->value == 1);
    assert(list.tail->value == 3);
    assert(int_list_find(&list, 2) != NULL);
    assert(int_list_find(&list, 99) == NULL);

    assert(int_list_remove_first(&list, 1) != 0);
    assert(list.head->value == 2);

    assert(int_list_remove_first(&list, 3) != 0);
    assert(list.tail->value == 2);

    assert(int_list_remove_first(&list, 2) != 0);
    assert(list.size == 0U);
    assert(list.head == NULL);
    assert(list.tail == NULL);

    assert(int_list_remove_first(&list, 99) == 0);

    int_list_destroy(&list);
    return 0;
}
