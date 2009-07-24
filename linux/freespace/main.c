#include <stdio.h>
#include <string.h>
#include <sys/vfs.h>

double free_space(const char* path) {
    char tmppath[256];
    strcpy(tmppath, path);

    while (1) {
        printf("trying %s\n", tmppath);
        struct statfs fs_info;
        if (statfs(tmppath, &fs_info) == 0) {

            double ret = fs_info.f_bavail;
            ret *= fs_info.f_bsize;
            return ret;
        }

        // try parent directory
        char *cp = strrchr(tmppath, '/');
        if (cp) *cp = '\0';
        else return 0.0;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) return -1;

    char* path = argv[1];

    printf("free space on %s is %12.0f bytes\n", path, free_space(path));

    return 0;
}

