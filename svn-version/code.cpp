/*
    Compile with:
    svn propset svn:keywords "LastChangedDate LastChangedBy LastChangedRevision" code.cpp
*/

#include <stdio.h>

int main() {
    printf("last changed date: $LastChangedDate$\n");
    printf("last changed rev: $LastChangedRevision$\n");
    printf("last changed by: $LastChangedBy$\n");
}
