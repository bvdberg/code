#include <stdio.h>
#include <stdlib.h>

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

struct obj* obj_create(int id) {
    struct obj* o = (struct obj*)malloc(sizeof(struct obj));
    obj_setid(o, id);
    return o;
}
