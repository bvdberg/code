#include <stdio.h>

#include "obj.h"

int main()
{
    printf("size = %d\n", obj_size());    
    struct obj* o = obj_create(123);
    printf("o = %p\n", o);
    return 0;
}

