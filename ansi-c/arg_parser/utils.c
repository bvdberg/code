#include "utils.h"
#include <string.h>

#define MAX_ARGSLEN 512

int parseArgs(const char* cmd, const char* argstr, char* argv[], int maxargs) {
    static char tmp[MAX_ARGSLEN];
    int argc = 0;
    if (cmd[0] != 0) {
        argv[argc++] = (char*)cmd;
    }

    strcpy(tmp, argstr);
    char* cp = tmp;
    char* start = tmp;
    int inSingleQuote = 0;
    while (*cp != 0) {
        if (inSingleQuote) {
            if (*cp == '\'') {
                inSingleQuote = 0;
            }
        } else {
            if (*cp == '\'') {
                inSingleQuote = 1;
            } else if (*cp == ' ') {
                *cp = 0;
                argv[argc] = start;
                argc++;
                start = cp+1;
            }
        }
        cp++;
    }
    if (cp != start) {
        argv[argc] = start;
        argc++;
    }

    return argc;
}
