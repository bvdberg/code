#ifndef OBJ_H
#define OBJ_H

#include <sys/types.h>

struct obj;
 
/*
 * The compiler considers struct obj an incomplete type. Incomplete types
 * can be used in declarations.
 */
 
size_t obj_size(void);
 
int obj_setid(struct obj *, int);
 
int obj_getid(struct obj *, int *);

struct obj* obj_create(int);

#endif

