#include <stdio.h>
#include <stdint.h>
#include "list.h"

typedef struct {
    uint32_t value;
    struct list_tag tag;
} Value;

static void dump(list_t const list)
{
    printf("size = %d\n", (int)list_count(list));
    list_t node = list->next;
    while (node != list) {
        Value* v = list_entry(node, Value, tag);
        printf("  %d\n", v->value);
        node = node->next;
    }
}

int main(int argc, const char *argv[])
{
    Value v1 = { .value = 10 };
    Value v2 = { .value = 20 };
    Value v3 = { .value = 30 };

    struct list_tag values;
    list_init(&values);

    dump(&values);

    list_add_front(&values, &v1.tag);
    dump(&values);

    list_add_front(&values, &v2.tag);
    dump(&values);

    list_add_tail(&values, &v3.tag);
    dump(&values);

    list_remove(&v2.tag);
    dump(&values);

    while (!list_empty(&values)) {
        list_t head = list_pop_front(&values);
        Value* v = list_entry(head, Value, tag);
        printf("pop %d\n", v->value);
    }

    return 0;
}

