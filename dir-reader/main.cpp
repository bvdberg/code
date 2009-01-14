#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

// entry should:
// - be a regular file
// - .d_name len > 4 (.xml)
// - .d_name NOT start with .
// - .d_name ends with .xml
int matchFilename(const struct dirent* entry) {
    if (entry->d_type != DT_REG) return false;
    if (entry->d_name[0] == '.') return false;
    int len = strlen(entry->d_name);
    if (len < 5) return false;
    if (strcmp(&entry->d_name[len-4], ".xml") == 0) return true;
    return false;
}

void printdir(const char* name) {
    DIR* dir = opendir(name);
    if (dir == NULL) {
        perror("opendir");
        return;
    }
    struct dirent* current = readdir(dir);
    while (current != 0) {
        if (matchFilename(current) ) {
            printf("   %s\n", current->d_name);
        }
        current = readdir(dir);
    }
    closedir(dir);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("test <dir>\n");
        return 1;
    }
    printdir(argv[1]);
    return 0;
}

