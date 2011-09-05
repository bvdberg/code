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

int main(int argc, const char *argv[])
{
    ZZIP_DIR* dir = zzip_opendir("bd1");
    if (dir == NULL) {
        fprintf(stderr, "zzip_operdir failed\n");
        return -1;
    }

    int flags = 0;
    const char* name = "data";
    ZZIP_FILE* f1 = zzip_file_open(dir, name, flags);
    if (f1 == NULL) {
        fprintf(stderr, "zzip_file_open failed: %s\n", zzip_strerror_of(dir));
        return -1;
    }

    // ---- file info ----
    ZZIP_STAT stat;
    int err = zzip_file_stat(f1, &stat);
    if (err != 0) {
        fprintf(stderr, "zzip_file_stat failed: %s\n", zzip_strerror_of(dir));
        return -1;
    }
    printf("Stat: compr=%d,  csize=%d,  usize=%d  name=%s\n",
        stat.d_compr, stat.d_csize, stat.st_size, stat.d_name);

    printf("------ FILE %s size %d/%d ------\n", name, stat.d_csize, stat.st_size);
    char buf[4096];
    zzip_ssize_t size = zzip_file_read(f1, buf, 4096);
    if (size != stat.st_size) {
        fprintf(stderr, "only read %d bytes\n", size);
        return -1;
    }
    printf("%s\n", buf);
    printf("---------------------\n");

    err = zzip_file_close(f1);
    if (err != 0) {
        fprintf(stderr, "zzip_file_close failed: %s\n", zzip_strerror_of(dir));
        return -1;
    }

    err = zzip_closedir(dir);
    if (err != 0) {
        fprintf(stderr, "zzip_closedir failed: %s\n", zzip_strerror_of(dir));
        return -1;
    }

    return 0;
}
