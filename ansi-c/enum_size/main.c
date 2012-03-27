#include <stdio.h>

enum Result {
    RESULT_OK = 0,
    RESULT_IN_PROGRESS,
    RESULT_ABORT,
//    RESULT_END = 256,   // will increase sizeof() to 2
//    RESULT_END2 = 65536,    // will increase sizeof() to 4
} __attribute((packed))__;

int main(int argc, const char *argv[])
{
    enum Result result;
    printf("size = %d\n", sizeof(result));
    return 0;
}

