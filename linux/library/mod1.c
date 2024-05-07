#include "mod1.h"
#include "mod2.h"

int mod1_func(int a) {
    return 10 * a + mod2_func(a);
}

