#include <stdio.h>

// forward declare struct
typedef struct _Outer Outer;
typedef struct _Inner Inner;

struct _Outer {
    Outer* next;
    Inner* inner;
};

struct _Inner {
    Outer* outer;
};

int main(int argc, const char *argv[])
{
    Outer o;
    Outer b;
    o.next = &b;
    struct _Outer c;
    printf("sizeof(Inner)=%d\n", sizeof(Inner));
    printf("sizeof(Outer)=%d\n", sizeof(Outer));
    return 0;
}

