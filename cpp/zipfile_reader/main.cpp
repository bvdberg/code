#include <stdio.h>

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

void checkFile(ZZIP_DIR* dir, ZZIP_DIRENT* dirent)
{
    int flags = 0;
    ZZIP_FILE* f1 = zzip_file_open(dir, dirent->d_name, flags);
    if (f1 == NULL) {
        fprintf(stderr, "zzip_file_open failed: %s\n", zzip_strerror_of(dir));
        return;
    }

    // ---- file info ----
    char buf[4096];
    zzip_ssize_t size = zzip_file_read(f1, buf, 4096);
    if (size != dirent->st_size) {
        fprintf(stderr, "only read %d bytes\n", size);
        return;
    }
    //printf("%s\n", buf);
    //printf("---------------------\n");

    int err = zzip_file_close(f1);
    if (err != 0) {
        fprintf(stderr, "zzip_file_close failed: %s\n", zzip_strerror_of(dir));
        return;
    }
}


int main(int argc, const char *argv[])
{
    ZZIP_DIR* dir = zzip_opendir("bd1");
    if (dir == NULL) {
        fprintf(stderr, "zzip_operdir failed: TODO (errno?)\n");
        return -1;
    }

    // scan files
    ZZIP_DIRENT* dirent = zzip_readdir(dir);
    while (dirent) {
        printf("name %s, compression %d, size %d/%d\n",
            dirent->d_name, dirent->d_compr, dirent->d_csize, dirent->st_size);

        checkFile(dir, dirent);
        dirent = zzip_readdir(dir);
    }

    int err = zzip_closedir(dir);
    if (err != 0) {
        fprintf(stderr, "zzip_closedir failed: %s\n", zzip_strerror_of(dir));
        return -1;
    }

    return 0;
}

