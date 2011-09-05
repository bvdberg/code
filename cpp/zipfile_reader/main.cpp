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
/*
    // we are just going to read the first 10 characters out of the file
    char* array = new char[11];

    // what file endings to accept... "" means that you can use /test.zip/
    // in the file name - has to end with a zero
    static zzip_strings_t ext[] = { ".zip", ".pak", "", 0 };

    // these functions are exactly the same as the normal posix-IO ones
    // any of the directories in the path can be zip files... but they can't
    // be nested. eg this opens the zipfile.zip (or .pak) file that has a directory
    // called test holding the file data.dat
    ZZIP_FILE * file = zzip_open_ext_io ("bd1", O_RDONLY, 0, ext, NULL);
    if (file == NULL)
    {
        printf("could not open file\n");
        return 1;
    }


    if (zzip_fread(array , sizeof(char) , 10 , file)) {
        array[10] = '\0';
        printf("%s\n", array);
    } else {
        printf("could not read file\n");
    }
    zzip_close (file);

    delete[] array;
*/

    // -------------------
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
    int res = zzip_file_stat(f1, &stat);
    printf("Stat: compr=%d,  csize=%d,  usize=%d  name=%s\n",
        stat.d_compr, stat.d_csize, stat.st_size, stat.d_name);

    printf("------ FILE %s size %d/%d ------\n", name, stat.d_csize, stat.st_size);
    char buf[4096];
    zzip_ssize_t size = zzip_file_read(f1, buf, 4096);
    printf("%s\n", buf);
    printf("---------------------\n");

    int err = zzip_file_close(f1);
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
