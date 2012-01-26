#include <stdio.h>
#include <unistd.h>

// print colors only if writing to console. Skip if writing to file

#define ANSI_RED "\033[0;31m"
#define ANSI_GREEN "\033[0;32m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_NORMAL "\033[0m"

static void output(const char* str) {
    int stdout_is_tty = isatty(1);
    if (stdout_is_tty) {
        printf(ANSI_RED"TTY: %s"ANSI_NORMAL"\n", str);
    } else {
        printf("FILE: %s\n", str);
    }
}


int main(int argc, const char *argv[])
{
    output("foo");
    output("bar");
    output("faa");
    return 0;
}

