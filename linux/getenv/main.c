#include <stdio.h>
#include <stdlib.h>


static void show(const char* name)
{
    char* var = getenv(name);
    printf("[%s]=%s\n", name, var);
}

int main(int argc, const char *argv[])
{
    show("OLDPWD");
    show("bla");
    return 0;
}

