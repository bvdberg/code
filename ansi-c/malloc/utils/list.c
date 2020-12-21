/*
 * Copyright Ecotap B.V. 2020
 * All rights reserved
 */

#include <stdint.h>
#include <stdbool.h>
#include "utils/list.h"

void list_init(list_tag* src)
{
    src->next = src;
    src->prev = src;
}

void list_add_tail(list_tag* src, list_tag* item)
{
    list_tag* old_tail = src->prev;
    src->prev = item;
    item->next = src;
    item->prev = old_tail;
    old_tail->next = item;
}

void list_add_front(list_tag* src, list_tag* item)
{
    list_tag* old_head = src->next;
    old_head->prev = item;
    item->next = old_head;
    item->prev = src;
    src->next = item;
}

void list_remove(list_tag* item)
{
    list_tag* prev = item->prev;
    list_tag* next = item->next;
    prev->next = next;
    next->prev = prev;
    item->next = item;
    item->prev = item;
}

list_tag* list_pop_front(list_tag* item)
{
    list_tag* node = item->next;
    list_remove(node);
    return node;
}

list_tag* list_front(list_tag* item)
{
    // NOTE: should not be empty!
    list_tag* node = item->next;
    return node;
}

void list_move(list_tag* src, list_tag* dest)
{
    list_tag* node = src->next;

    while (node != src)
    {
        list_tag* tmp_node = node;
        node = node->next;
        list_remove(tmp_node);
        list_add_tail(dest, tmp_node);
    }
}

void list_swap(list_tag* list1, list_tag* list2)
{
    list_tag* p1 = list1->prev;
    list_tag* n1 = list1->next;
    list_tag* p2 = list2->prev;
    list_tag* n2 = list2->next;
    if (p1 == list1) p1 = list2;
    if (n1 == list1) n1 = list2;
    if (p2 == list2) p2 = list1;
    if (n2 == list2) n2 = list1;
    list1->prev = p2;
    list1->next = n2;
    list2->prev = p1;
    list2->next = n1;
    if (!list_empty(list1)) {
        p2->next = list1;
        n2->prev = list1;
    }
    if (!list_empty(list2)) {
        p1->next = list2;
        n1->prev = list2;
    }
}

uint64_t list_count(const list_tag* src)
{
    uint64_t count = 0;
    list_tag* node = src->next;

    while (node != src)
    {
        count++;
        node = node->next;
    }
    return count;
}

bool list_empty(const list_tag* src)
{
    return src->prev == src;
}

