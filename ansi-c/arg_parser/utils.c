#include "utils.h"
#include <string.h>

#define MAX_ARGSLEN 512

//#include <stdio.h>

int parseArgs(const char* cmd, const char* argstr, char* argv[], int maxargs) {
    //printf("INPUT [%s]\n", argstr);
    static char tmp[MAX_ARGSLEN];
    int argc = 0;
    if (cmd[0] != 0) {
        argv[argc++] = (char*)cmd;
    }

    char* op = tmp;
    const char* ip = argstr;
    char* start = op;
    int inSingleQuote = 0;
    while (*ip != 0) {
        //printf(" '%c' %d\n", *ip, inSingleQuote);
        if (inSingleQuote) {
            if (*ip == '\'') {
                inSingleQuote = 0;
                ip++;
            } else {
                *op++ = *ip++;
            }
        } else {
            if (*ip == '\'') {
                inSingleQuote = 1;
                ip++;
                continue;
            }
            if (*ip == ' ') {
                if (op == start) {
                    ip++;
                    continue;
                }
                *op++ = 0;
                ip++;
                argv[argc] = start;
                argc++;
                start = op;
            } else {
                *op++ = *ip++;
            }
        }
    }
    if (op != start) {
        *op = 0;
        argv[argc] = start;
        argc++;
    }
    argv[argc] = 0;

    return argc;
}
