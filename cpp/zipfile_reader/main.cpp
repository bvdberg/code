#include <stdio.h>
#include <string.h>
#include <zzip/zzip.h>

void checkFile(ZZIP_DIR* dir, ZZIP_DIRENT* dirent, const char* filename)
{
    if (strcmp(filename, dirent->d_name) != 0) return;

    int flags = 0;
    ZZIP_FILE* f1 = zzip_file_open(dir, dirent->d_name, flags);
    if (f1 == NULL) {
        fprintf(stderr, "zzip_file_open failed: %s\n", zzip_strerror_of(dir));
        return;
    }

    // read file
    char buf[4097];
    int total = dirent->st_size;
    int bytes = 0;
    printf("---------------------\n");
    while (bytes != total) {
        zzip_ssize_t size = zzip_file_read(f1, buf, 4096);
        bytes += size;
        buf[size] = 0;
        printf("%s", buf);
        //printf("read %4d  %6d/%d\n", size, bytes, total);
    }
    printf("\n---------------------\n");

    int err = zzip_file_close(f1);
    if (err != 0) {
        fprintf(stderr, "zzip_file_close failed: %s\n", zzip_strerror_of(dir));
        return;
    }
}


int main(int argc, const char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return -1;
    }
    const char* innerFile = "";
    if (argc == 3) innerFile = argv[2];

    zzip_error_t errcode;
    ZZIP_DIR* dir = zzip_dir_open(argv[1], &errcode);
    if (dir == NULL) {
        fprintf(stderr, "zzip_operdir failed: %s\n", zzip_strerror(errcode));
        return -1;
    }

    // scan files
    ZZIP_DIRENT* dirent = zzip_readdir(dir);
    while (dirent) {
        printf("%s, compression %d, size %d/%d\n",
            dirent->d_name, dirent->d_compr, dirent->d_csize, dirent->st_size);

        checkFile(dir, dirent, innerFile);
        dirent = zzip_readdir(dir);
    }

    int err = zzip_closedir(dir);
    if (err != 0) {
        fprintf(stderr, "zzip_closedir failed: %s\n", zzip_strerror_of(dir));
        return -1;
    }

    return 0;
}

