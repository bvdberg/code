#include <stdio.h>
#include <libgen.h>

//char *dirname(char *path);
//char *basename(char *path);


int main(int argc, char *argv[])
{
    printf("%s\n", argv[0]);
    printf("-> dirname: %s\n", dirname(argv[0]));
    printf("-> basename: %s\n", basename(argv[0]));
    return 0;
}

