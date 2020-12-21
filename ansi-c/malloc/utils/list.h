/*
 * Copyright Ecotap B.V. 2020
 * All rights reserved
 */

#ifndef LIST_H
#define LIST_H

#include <stdint.h>
#include <stdbool.h>

typedef struct list_tag_ list_tag;

struct list_tag_
{
    list_tag* prev;
    list_tag* next;
};

#define to_container(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

void list_init(list_tag* src);

void list_add_tail(list_tag* src, list_tag* item);

void list_add_front(list_tag* src, list_tag* item);

list_tag* list_pop_front(list_tag* item);

list_tag* list_front(list_tag* item);

void list_remove(list_tag* item);

void list_move(list_tag* src, list_tag* dest);

void list_swap(list_tag* list1, list_tag* list2);

uint64_t list_count(const list_tag* src);

bool list_empty(const list_tag* src);

#endif
