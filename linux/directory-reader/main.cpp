#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

void printdir(const char* name) {
    DIR* dir = opendir(name);
    if (dir == NULL) {
        perror("opendir");
        return;
    }
    struct dirent* dir2 = readdir(dir);
    while (dir2 != 0) {
        printf("dtype = 0x%02x  name = %s\n", dir2->d_type, dir2->d_name);
        if (dir2->d_type == DT_DIR) {
            if (strcmp(dir2->d_name, ".") != 0 && strcmp(dir2->d_name, "..") != 0) {
                //printdir(dir2->d_name);
            }
        }
        dir2 = readdir(dir);
    }
    closedir(dir);
}

int main(int argc, const char *argv[])
{
    printdir(argv[1]);
    return 0;
}

