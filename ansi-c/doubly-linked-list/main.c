#include <stdio.h>

#include "list.h"

struct kool_list {
    struct list_head list;
    int number;
};

int main() {
    int i;
    struct list_head *pos;

    struct kool_list mylist;
    INIT_LIST_HEAD(&mylist.list);
    
    for(i=0; i<5; i++) {
        struct kool_list* tmp = (struct kool_list *)malloc(sizeof(struct kool_list));
        tmp->number = i;

//        list_add(&(tmp->list), &(mylist.list));
        list_add_tail(&(tmp->list), &(mylist.list));
    }

    list_for_each(pos, &mylist.list) {
        struct kool_list* tmp = list_entry(pos, struct kool_list, list);    
        printf("number = %d\n", tmp->number);
    }
    // free all

    return 0;
}

