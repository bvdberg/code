#include <stdio.h>

void func_non_static() {
    printf("world\n");
}

static void func_static() {
    printf("hallo\n");
}

int hidden_i __attribute__((visibility("hidden")));
int non_hidden_i;
static int static_i;
#if 0
void func_hidden() __attribute__((visibility("hidden")) {
    printf("!\n");
}
#endif

int main(int argc, const char *argv[])
{
    return 0;
}

