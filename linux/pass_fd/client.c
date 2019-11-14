// From:
// https://openforums.wordpress.com/2016/08/07/open-file-descriptor-passing-over-unix-domain-sockets/

/*
    NOTES:
    - reading from file before passing keeps current offset in file on other side.
    - receiver can use lseek to reset offset again
    - access rights are only checked at open(), not when sent
        sudo ./send /root/file    can be received by non-root client
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while(0)

static void send_fd(int socket, const int *fds, int n) {
    char buf[CMSG_SPACE(n * sizeof(int))];
    memset(buf, '\0', sizeof(buf));
    char dup[256];
    struct iovec io = { .iov_base = &dup, .iov_len = sizeof(dup) };

    struct msghdr msg = {0};
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(n * sizeof(int));

    memcpy ((int *) CMSG_DATA(cmsg), fds, n * sizeof (int));

    printf("size %lu\n", sizeof(msg));
    if (sendmsg (socket, &msg, 0) < 0)
        handle_error ("Failed to send message");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf (stderr, "Usage: %s <file-name1> <file-name2>\n", argv[0]);
        exit (1);
    }

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1)
        handle_error ("Failed to create socket");

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/fd-pass.socket", sizeof(addr.sun_path) - 1);

    int fds[2];
    fds[0] = open(argv[1], O_RDONLY);
    if (fds[0] < 0) handle_error ("Failed to open file 1 for reading");
    fprintf (stdout, "Opened fd %d in client\n", fds[0]);

    fds[1] = open(argv[2], O_RDONLY);
    if (fds[1] < 0) handle_error ("Failed to open file 2 for reading");
    fprintf (stdout, "Opened fd %d in client\n", fds[1]);

    char buf[3];
    int numread = read(fds[0], buf, 3);
    if (numread != 3) handle_error("read");

    if (connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
        handle_error ("Failed to connect to socket");

    send_fd (sfd, fds, 2);

    close(fds[0]);
    close(fds[1]);
    close(sfd);

    exit(EXIT_SUCCESS);
}

