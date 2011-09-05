#include <stdio.h>
#include <string.h>

/*
    Possible libs:
        libzipios
        libzzip <- lightweight
        libzip (also creation, modification)
        libarchive (also creation, other formats)
        libghc6-zip-archive
*/

#define LIBZZIP

#ifdef LIBZZIP
#include <zzip/zzip.h>
#endif

void checkFile(ZZIP_DIR* dir, ZZIP_DIRENT* dirent, const char* filename)
{
    int flags = 0;
    ZZIP_FILE* f1 = zzip_file_open(dir, dirent->d_name, flags);
    if (f1 == NULL) {
        fprintf(stderr, "zzip_file_open failed: %s\n", zzip_strerror_of(dir));
        return;
    }
    bool print = false;
    if (strcmp(filename, dirent->d_name) == 0) print = true;

    // read file
    char buf[4097];
    int total = dirent->st_size;
    int bytes = 0;
    if (print) printf("---------------------\n");
    while (bytes != total) {
        zzip_ssize_t size = zzip_file_read(f1, buf, 4096);
        bytes += size;
        buf[size] = 0;
        if (print) printf("%s", buf);
        //printf("read %4d  %6d/%d\n", size, bytes, total);
    }
    if (print) printf("\n---------------------\n");

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

    ZZIP_DIR* dir = zzip_opendir(argv[1]);
    if (dir == NULL) {
        fprintf(stderr, "zzip_operdir failed: TODO (errno?)\n");
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

