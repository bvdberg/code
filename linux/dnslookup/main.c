#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

extern int h_errno;

void obsolute_lookup(const char* host)
{
    printf("OLD:\n");
    char buf[1024];
    memset(buf, 0, 1024);
    int err;
    struct hostent h, *hp;
    int ret = gethostbyname_r(host, &h, buf, sizeof(buf), &hp, &err);
    if (ret) {
        perror("gethostbyname_r");
        return;
    }

    printf("host = '%s'\n", h.h_name);
    int i;
    printf("ip = ");
    for (i=0; i<h.h_length; i++) {
        if (i!= 0) printf(".");
        printf("%d", (unsigned char)h.h_addr_list[0][i]);
    }
    printf("\n");
}


void print_addrinfo(int index, struct addrinfo* info)
{
    printf("--- INFO[%d] ---\n", index);
    printf("ai_flags=%d\n", info->ai_flags);
    printf("ai_family=%d\n", info->ai_family);
    printf("ai_socktype=%d\n", info->ai_socktype);
    printf("ai_protocol=%d\n", info->ai_protocol);
    printf("ai_addrlen=%d\n", info->ai_addrlen);


    //struct sockaddr *sa;    /* input */
    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
    if (getnameinfo(info->ai_addr, sizeof(struct sockaddr), hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV) == 0) {
        printf("host=%s, serv=%s\n", hbuf, sbuf);
    }
    // HUH?

    printf("name = '%s'\n", info->ai_canonname);
    if (info->ai_next) print_addrinfo(index+1, info->ai_next);
}


void new_lookup(const char* host)
{
    printf("NEW:\n");
    struct addrinfo* info = NULL;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    int err = getaddrinfo(host, NULL, &hints, &info);
    if (err) {
        printf("ERROR: %s\n", gai_strerror(err));
    } else {
        print_addrinfo(0, info);
    }
    freeaddrinfo(info);

}


int main(int argc, const char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s [host]\n", argv[0]);
        return -1;
    }
    const char* host = argv[1];

    obsolute_lookup(host);
    new_lookup(host);

    return 0;
}

