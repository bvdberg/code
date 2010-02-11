#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// Determine if stdin/stdout are pipes, files or normal terminals
// ./test < Makefile
// ./test < /dev
// ./test > zz

int main(int argc, const char *argv[])
{
    // 0=stdin, 1=stdout, 2=stderr
    struct stat buf;
    int error = fstat(0, &buf);
    if (error) { perror("fstat"); return -1; }
    mode_t m = buf.st_mode;
    fprintf(stderr, " stdin: regular=%d  chardev=%d  blockdev=%d  fifo=%d  dir=%d\n",
        S_ISREG(m), S_ISCHR(m), S_ISBLK(m), S_ISFIFO(m), S_ISDIR(m));

    error = fstat(1, &buf);
    if (error) { perror("fstat"); return -1; }
    m = buf.st_mode;
    fprintf(stderr, "stdout: regular=%d  chardev=%d  blockdev=%d  fifo=%d  dir=%d\n",
        S_ISREG(m), S_ISCHR(m), S_ISBLK(m), S_ISFIFO(m), S_ISDIR(m));

    return 0;
}

