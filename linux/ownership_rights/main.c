#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
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
    S_ISREG(m)  is it a regular file?
    S_ISDIR(m)  directory?
    S_ISCHR(m)  character device?
    S_ISBLK(m)  block device?
    S_ISFIFO(m) FIFO (named pipe)?
    S_ISLNK(m)  symbolic link? (Not in POSIX.1-1996.)
    S_ISSOCK(m) socket? (Not in POSIX.1-1996.)
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

    mode_t m = statbuf.st_mode;
    printf("  -> reg=%d  dir=%d  chr=%d  blk=%d  fifo=%d  link=%d  sock=%d\n",
        S_ISREG(m), S_ISDIR(m), S_ISCHR(m), S_ISBLK(m), S_ISFIFO(m), S_ISLNK(m), S_ISSOCK(m));
}


static void printRights(mode_t mode) {
    mode_t mode2 = mode & (0x3FF);
    printf("  0x%x  (0x%x,  Octal: %o)\n", mode, mode2, mode2);
    char buffer[11];
    memset(buffer, '-', sizeof(buffer));
    buffer[sizeof(buffer)-1] = 0;
    char* cp = &buffer[0];
    cp++;
    if (S_IRUSR & mode) *cp = 'r'; cp++;
    if (S_IWUSR & mode) *cp = 'w'; cp++;
    if (S_IXUSR &mode) *cp = 'x'; cp++;
    if (S_IRGRP & mode) *cp = 'r'; cp++;
    if (S_IWGRP & mode) *cp = 'w'; cp++;
    if (S_IXGRP &mode) *cp = 'x'; cp++;
    if (S_IROTH & mode) *cp = 'r'; cp++;
    if (S_IWOTH & mode) *cp = 'w'; cp++;
    if (S_IXOTH &mode) *cp = 'x'; cp++;
    printf("rights: %s\n", buffer);
}


int main(int argc, const char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: test [file]\n");
        return 1;
    }
    const char* filename = argv[1];
    struct stat statbuf;
    int err = lstat(filename, &statbuf);
    if (err) {
        perror("stat");
        return 1;
    }

    printStat(filename, statbuf);
    printRights(statbuf.st_mode);

    return 0;
}

