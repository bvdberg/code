#include <stdio.h>
#include <string.h>
#include <netdb.h>

extern int h_errno;


int main(int argc, const char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s [host]\n", argv[0]);
        return -1;
    }
    const char* host = argv[1];

    char buf[1024];
    memset(buf, 0, 1024);
    int err;
    struct hostent h, *hp;
    int ret = gethostbyname_r(host, &h, buf, sizeof(buf), &hp, &err);
    if (ret) {
        perror("gethostbyname_r");
        return 1;
    }

    printf("host = '%s'\n", h.h_name);
    int i;
    printf("ip = ");
    for (i=0; i<h.h_length; i++) {
        if (i!= 0) printf(".");
        printf("%d", (unsigned char)h.h_addr_list[0][i]);
    }
    printf("\n");
    return 0;
}

