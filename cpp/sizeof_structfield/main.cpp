#include <stdio.h>

// NOTE: you dont need an instance to do sizeof for struct fields

struct mystruct {
    short num;
};


class Myclass {
public:
    int num;
};


int main() {
    struct mystruct* ms;
    Myclass* mc;
    printf("struct: sizeof num = %d\n", sizeof(ms->num));
    printf("class: sizeof num = %d\n", sizeof(mc->num));
    return 0;
}

