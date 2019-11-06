// From:
// https://openforums.wordpress.com/2016/08/07/open-file-descriptor-passing-over-unix-domain-sockets/

#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while(0)

#if 0
static void dump(const uint8_t* data, unsigned len) {
    for (unsigned i=0; i<len; i++) {
        printf("  %02x", data[i]);
        if (i%16 == 15) printf("\n");
    }
    if (len%16 != 15) printf("\n");
}
#endif

static int* recv_fd(int socket, int n) {
    int *fds = malloc (n * sizeof(int));
    char buf[CMSG_SPACE(n * sizeof(int))], dup[256];
    memset(buf, '\0', sizeof(buf));
    struct iovec io = { .iov_base = &dup, .iov_len = sizeof(dup) };

    struct msghdr msg = {0};
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    if (recvmsg (socket, &msg, 0) < 0)
        handle_error ("Failed to receive message");

    //dump((const uint8_t*)&msg, sizeof(msg));

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    memcpy (fds, (int *) CMSG_DATA(cmsg), n * sizeof(int));
    return fds;
}

int main(int argc, char *argv[]) {
    printf("pid %u: ls -al /proc/%u/fd\n", getpid(), getpid());

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) handle_error ("Failed to create socket");

    if (unlink ("/tmp/fd-pass.socket") == -1 && errno != ENOENT)
        handle_error ("Removing socket file failed");

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/fd-pass.socket", sizeof(addr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
        handle_error ("Failed to bind to socket");

    if (listen(sfd, 5) == -1)
        handle_error ("Failed to listen on socket");

    int cfd = accept(sfd, NULL, NULL);
    if (cfd == -1)
        handle_error ("Failed to accept incoming connection");

    int* fds = recv_fd (cfd, 2);

    for (int i=0; i<2; ++i) {
        fprintf (stdout, "Reading from passed fd %d\n", fds[i]);
        char buffer[256];
        ssize_t nbytes;
        while ((nbytes = read(fds[i], buffer, sizeof(buffer))) > 0) {
            int written = write(1, buffer, nbytes);
            if (written != nbytes) handle_error("cannot write");
        }
        *buffer = '\0';
        //close(fds[i]);  // NOTE: need to close, here
    }

    if (close(cfd) == -1)
        handle_error ("Failed to close client socket");

    printf("waiting after (pid %u)\n", getpid());
    sleep(20);

    close(sfd);
    return 0;
}

