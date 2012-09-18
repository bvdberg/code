#include <stdio.h>

namespace A {
void myfunction() {
    printf("A::myfunction()\n");
}
}

namespace B {
void myfunction() {
    printf("B::myfunction()\n");
}
}

using namespace A;
using namespace B;

int main(int argc, const char *argv[])
{
    myfunction(); 
    return 0;
}

