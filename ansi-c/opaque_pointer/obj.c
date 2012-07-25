#include <stdio.h>

#include "obj.h"
 
struct obj {
    int id;
    int data;
};
 
/*
 * The caller will handle allocation.
 * Provide the required information only
 */
 
size_t obj_size(void) {
    return sizeof(struct obj);
}
 
int obj_setid(struct obj *o, int i) {
    if (o == NULL) return -1;
    o->id = i;
    return 0;
}
 
int obj_getid(struct obj *o, int *i) {
    if (o == NULL) return -1;
    *i = o->id;
    return 0;
}

