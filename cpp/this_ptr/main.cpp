#include <stdio.h>

class Object {
public:
    Object() {
        printf("this = %p\n", this);
    }
private:
    int num;
};

int main(int argc, const char *argv[])
{
    Object obj1;    // will get decreasing this ptrs (stack grows down)
    Object obj2;
    Object obj3;
    Object obj4;

    for (int i=0; i<10; i++) {
        Object obj5;        // will get same this* (on stack)
    }
    return 0;
}

