#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
    printf("start\n");
    sleep(4);
#ifdef CRASH
    abort();  // to cause crash
#endif
    printf("end\n");
    return 0;
}

