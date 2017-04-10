#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

static void readdir_test(const char* dirname)
{
    printf("readdir:\n");
	DIR *dir = opendir(dirname);
    if (dir == NULL)
    {
        fprintf(stderr, "error opening directory %s: %s", dirname, strerror(errno));
        exit(-1);
    }
    struct dirent* current = readdir(dir);
    while (current != 0)
    {
        printf("  %s\n", current->d_name);
        current = readdir(dir);
    }
    closedir(dir);
}
static void scandir_test(const char* dirname)
{
    printf("scandir:\n");
	struct dirent **namelist;
	int n = scandir(dirname, &namelist, NULL, alphasort);
	if (n < 0) {
		fprintf(stderr, "error opening  directory %s: %s\n", dirname, strerror(errno));
        exit(-1);
    }
    while (n--) {
        printf("  %s\n", namelist[n]->d_name);
        free(namelist[n]);
    }
    free(namelist);
}


int main(int argc, const char *argv[])
{

    const char* dirname = argv[1];
    readdir_test(dirname);
    scandir_test(dirname);

    return 0;
}

