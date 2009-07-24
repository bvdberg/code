#include <stdio.h>
#include "normal.h"

void Derived::myprint() {
    for (int i=0; i<10; i++) printf("OVERRIDE!\n");
}

