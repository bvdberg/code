/*
    Compile with:
    svn propset svn:keywords "LastChangedDate LastChangedBy LastChangedRevision" code.cpp
*/

#include <stdio.h>
//$LastChangedDate: 2008-06-10 07:41:36 +0200 (Tue, 10 Jun 2008) $

int main() {
    printf("last changed date: $LastChangedDate: 2008-06-10 07:41:36 +0200 (Tue, 10 Jun 2008) $\n");
    printf("last changed rev: $LastChangedRevision: -1M $\n");
    printf("last changed by: $LastChangedBy: (local) $\n");
}
