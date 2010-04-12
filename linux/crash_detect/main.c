#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
    printf("start\n");
    sleep(2);
    //abort();  // to cause crash
    printf("end\n");
    return 0;
}

