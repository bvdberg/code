#include <stdio.h>

int myfunc(int num) {
    return 10*num;
}

int main(int argc, const char *argv[])
{
    int value1 = {
        myfunc(argc)
    };
    printf("value1=%d\n", value1);

    int value2 = ({
        static int b;
        myfunc(argc);
    });
    printf("value2=%d\n", value2);
    return 0;
}

