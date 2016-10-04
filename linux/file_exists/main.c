#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


static int file_exists(const char* filename) {
    struct stat statbuf;
    if (stat(filename, &statbuf) != 0) return 0;
    return 1;
}

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        printf("usage: %s <filename>\n", argv[0]);
        return -1;
    }

    const char* filename = argv[1];
    int exists = file_exists(filename);
    printf("%s does %s\n", filename, exists ? "exst" : "not exist");
    return 0;
}

