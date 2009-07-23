#include <stdio.h>

/* library's utility function */
void
say_something_nice()
{
    printf("You talkin' to me? I said, you talkin' to me?\n");
}

/* library's cleanup function */
void
_fini()
{
    printf("Cleaning out library 'lib2.so'\n");
}
