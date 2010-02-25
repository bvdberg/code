#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
/*
 struct stat {
     dev_t     st_dev;     // ID of device containing file
     ino_t     st_ino;     // inode number
     mode_t    st_mode;    // protection
     nlink_t   st_nlink;   // number of hard links
     uid_t     st_uid;     // user ID of owner
     gid_t     st_gid;     // group ID of owner
     dev_t     st_rdev;    // device ID (if special file)
     off_t     st_size;    // total size, in bytes
     blksize_t st_blksize; // blocksize for filesystem I/O
     blkcnt_t  st_blocks;  // number of blocks allocated
     time_t    st_atime;   // time of last access
     time_t    st_mtime;   // time of last modification
     time_t    st_ctime;   // time of last status change
 };
*/

static void printStat(const char* filename, struct stat statbuf) {
    printf("file='%s'\n", filename);
    printf("  st_dev=%lld\n", statbuf.st_dev);
    printf("  st_ino=%ld\n", statbuf.st_ino);
    printf("  st_mode=0x%x\n", statbuf.st_mode);
    printf("  st_nlink=%d\n", statbuf.st_nlink);
    printf("  st_uid=%d\n", statbuf.st_uid);
    printf("  st_gid=%d\n", statbuf.st_gid);
    printf("  st_rdev=%lld\n", statbuf.st_rdev);
    printf("  st_size=%ld\n", statbuf.st_size);
    printf("  st_blksize=%ld\n", statbuf.st_blksize);
    printf("  st_blocks=%ld\n", statbuf.st_blocks);
    printf("  st_atime=%ld\n", statbuf.st_atime);
    printf("  st_mtime=%ld\n", statbuf.st_mtime);
    printf("  st_ctime=%ld\n", statbuf.st_ctime);

}

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: test [file]\n");
        return 1;
    }
    const char* filename = argv[1];
    struct stat statbuf;
    int err = stat(filename, &statbuf);
    if (err) {
        perror("stat");
        return 1;
    }

    printStat(filename, statbuf);

    return 0;
}

