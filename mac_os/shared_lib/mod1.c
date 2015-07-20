#include "mod1.h"
#include <stdio.h>

int mod1_func(int a) {
    printf("%s()\n", __func__);
    return 10 * a;
}

