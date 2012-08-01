#include <stdio.h>

typedef struct {
    int a;
    int b;
    union {
        // NOTE: without union name, this vars are 'unreachable'
        int a;
        int b;
        int z;
    };
    //} t;
} MyStruct;

static MyStruct m;

int main()
{
    printf("size=%d\n", sizeof(m));

    m.a = 10;
    m.b = 20;
    m.z = 30;
    //printf("%d, %d\n", m.a, m.t.z);
    printf("%d, %d\n", m.a, m.z);  // <- allowed, but union.a is 'unreachable'
    
    return 0;
}

