#include <stdio.h>
#include <unistd.h>
#include <string.h>

// try dup-ping file description and closing one.
// can we still write to other? -> YES


static void test(int fd, const char* msg)
{
    ssize_t written = write(3, msg, strlen(msg));
    fprintf(stderr, "wrote %d: %d bytes\n", fd, written);
}

int main(int argc, const char *argv[])
{
    const char* msg = "The quick brown fox jumped over the lazy gate.\n";
    test(1, msg);

    int copy = dup(1);
    close(1);

    test(3, msg);

    return 0;
}

