#include <stdio.h>

#include "git_version.h"

int main(int argc, const char *argv[])
{
    printf("git-version='%s'\n", git_version);
    return 0;
}
