#include <stdio.h>

/* library's initialization function. must be named '_init'. */
void _init()
{
    printf("Initializing library 'lib1.so'\n");
}

/* library's utility function */
void say_something_nice()
{
    printf("If you need to shoot - shoot. Don't talk.\n");
}
