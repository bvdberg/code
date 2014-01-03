#include <stdio.h>

enum class Type {
    foo,
    bar,
};

int main(int argc, const char *argv[])
{
    Type t = Type::foo;
    return 0;
}

