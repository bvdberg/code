#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct list_tag
{
    struct list_tag *prev;
    struct list_tag *next;
};

#define to_container(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

typedef struct list_tag * list_t;

void list_init(list_t const src);

void list_add_tail(list_t const src,
                   list_t const item);

void list_add_front(list_t const src,
                    list_t const item);

list_t list_pop_front(list_t const item);

void list_remove(list_t const item);

void list_move(list_t src,
               list_t const dest);

uint64_t list_count(list_t const src);

bool list_empty(list_t const src);

#ifdef __cplusplus
}
#endif

#endif
