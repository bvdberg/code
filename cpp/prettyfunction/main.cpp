#include <stdio.h>

class MyClass {
public:
    static int bla(int a, int c) {
        printf("__PRETTY_FUNCTION__ = '%s'\n", __PRETTY_FUNCTION__);
        printf("__FUNCTION__ = '%s'\n", __FUNCTION__);
        return a*c;
    }
};
int main() {
    MyClass::bla(10,20);
    return 0;
}

