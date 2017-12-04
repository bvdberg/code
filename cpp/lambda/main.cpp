#include <stdio.h>

template<typename Functor>
static void f(Functor functor) {
    printf("f -> %d\n", functor(10));
}

static int g(int x) {
    return x * x;
}

int main(int argc, const char *argv[])
{
    int result = [](int a,int b) -> int { return a + b; }(2,4);
    printf("%d\n", result);

    auto func = [](int a, int b) -> int { return a + b; };
    printf("%d\n", func(4, 3));

    auto lambda = [](int x) { return x * 5; };
    f(lambda);
    f(g);

    unsigned total = 0;
    auto sum = [&total](int x) { total += x; return x * 3; };
    f(sum);
    f(sum);
    printf("total %d\n", total);

    return 0;
}

