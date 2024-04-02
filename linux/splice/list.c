/* Copyright 2013-2024 Bas van den Berg
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include <stdbool.h>
#include "list.h"

void list_init(list_t const src)
{
    src->next = src;
    src->prev = src;
}

void list_add_tail(list_t const src,
                   list_t const item)
{
    list_t const old_tail = src->prev;
    src->prev = item;
    item->next = src;
    item->prev = old_tail;
    old_tail->next = item;
}

void list_add_front(list_t const src,
                    list_t const item)
{
    list_t const old_head = src->next;
    old_head->prev = item;
    item->next = old_head;
    item->prev = src;
    src->next = item;
}

void list_remove(list_t const item)
{
    list_t prev = item->prev;
    list_t next = item->next;
    prev->next = next;
    next->prev = prev;
    item->next = item;
    item->prev = item;
}

list_t list_pop_front(list_t const item)
{
    list_t node = item->next;
    list_remove(node);
    return node;
}

void list_move(list_t src,
               list_t const dest)
{
    list_t node = src->next;

    while (node != src)
    {
        list_t const tmp_node = node;
        node = node->next;
        list_remove(tmp_node);
        list_add_tail(dest, tmp_node);
    }
}

uint64_t list_count(list_t const src)
{
    uint64_t count = 0;
    list_t node = src->next;

    while (node != src)
    {
        count++;
        node = node->next;
    }
    return count;
}

bool list_empty(list_t const src)
{
    return src->prev == src;
}

