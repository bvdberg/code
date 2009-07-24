#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    struct stat stats;
    int error = stat(argv[1], &stats);
    if (error) {
        perror("stat");
    } else {
        printf("STAT size=%d  mtime=%d\n", stats.st_size, stats.st_mtime);
    }

    return 0;
}

