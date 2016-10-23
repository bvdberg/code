#include <stdio.h>
#include <errno.h>
#include <sys/utsname.h>
#include <netdb.h>

int main(int argc, const char *argv[])
{
    struct utsname un;
    int len;
    if (uname(&un) != 0) {
        perror("uname");
        return -1;
    }
    printf("sysname = %s\n", un.sysname);
    printf("nodename = %s\n", un.nodename);
    printf("release = %s\n", un.release);
    printf("version = %s\n", un.version);
    printf("machine = %s\n", un.machine);

    struct hostent *hp;
    if ((hp = gethostbyname(un.nodename)) == NULL) {
        printf("unable to get system name\n");
        printf("REMOTE = %s\n", un.nodename);
    } else {
        printf("REMOTE = %s\n", hp->h_name);
    }
    
    return 0;
}

