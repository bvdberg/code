#include <stdio.h>

static int s_int_a;
static const char* s_string = "foobar";
static int s_int_b;

int main(int argc, const char *argv[])
{
    int a = 10;
    int c;
    static int b = 20;
    const int d = 30;
    static const int e = 30;
    printf("static int a = %p\n", &s_int_a);
    printf("static int b = %p\n", &s_int_b);
    printf("static const char* = %p\n", s_string);
    printf("main = %p\n", &main);
    printf("int main.a = %p\n", &a);
    printf("static int main.b = %p\n", &b);
    printf("int main.c = %p\n", &c);
    printf("const int main.d = %p\n", &d);
    printf("static const int main.e = %p\n", &e);
    return 0;
}

