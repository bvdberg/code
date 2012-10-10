#include <stdio.h>

static int calc(int a) {
    return 10*a;
}

int main(int argc, const char *argv[])
{
    int nums[2] = { calc(1), calc(2) };     // ok
    //static int nums[2] = { calc(1), calc(2) };  // not allowed
    printf("nums = %d, %d\n", nums[0], nums[1]);

    return 0;
}

