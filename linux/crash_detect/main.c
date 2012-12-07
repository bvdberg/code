#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
    printf("application started, %d args\n", argc);
    int i;
    for (i=0; i<argc; i++) printf("arg[%d] = '%s'\n", i, argv[i]);
    sleep(4);
#ifdef CRASH
    abort();  // to cause crash
#endif
    printf("end\n");
    return 0;
}

