#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#define MAX_PATH 512

const char* find_bin(const char* name) {
    static char result[MAX_PATH];
    struct stat statbuf;

    char *dup = strdup(getenv("PATH"));
    char *s = dup;
    char *p = NULL;
    do {
        p = strchr(s, ':');
        if (p != NULL) {
            p[0] = 0;
        }
        sprintf(result, "%s/%s", s, name);
        //printf("Path in $PATH: %s - %s\n", s, result);
        if (stat(result, &statbuf) == 0) {
            //printf("FOUND %s\n", result);
            free(dup);
            return result;
        }
        s = p + 1;
    } while (p != NULL);

    free(dup);
    return NULL;
}

int main(int argc, const char *argv[])
{
    if (argc == 1) {
        printf("usage: %s [binary]\n", argv[0]);
        return 1;
    }
    printf("Result: %s\n", find_bin(argv[1]));
    return 0;
}

