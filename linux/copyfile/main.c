#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sendfile.h>

// NOTE: fd_out must be a socket

int main(int argc, const char *argv[])
{
    if (argc != 3) return -1;

    const char* infile = argv[1];
    const char* outfile = argv[2];

    int fd_in = open(infile, O_RDONLY);
    if (fd_in == -1) {
        perror("open infile");
        return -1;
    }

    int fd_out = open(outfile, O_WRONLY|O_CREAT|O_TRUNC, 0600);
    if (fd_out == -1) {
        perror("open outfile");
        return -1;
    }

    struct stat statbuf;
    int res = fstat(fd_in, &statbuf);
    if (res == -1) {
        perror("fstat");
        goto out;
    }
    ssize_t size = statbuf.st_size;
    printf("size=%d bytes\n", size);

    // normal read-write method
    ssize_t copied = 0;
    while (copied != size) {
        char buffer[2000];
        int n_read = read(fd_in, buffer, sizeof(buffer));
        if (n_read == -1) {
            perror("read");
            goto out;
        }
        if (n_read == 0) break;

        int n_written = write(fd_out, buffer, n_read);
        if (n_written != n_read) {
            perror("write");
            goto out;
        }
    }
out:
    close(fd_out);
    close(fd_in);
    return 0;
}

