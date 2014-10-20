#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

//TEST WITH: ps -eo uname:20,pid,pcpu,pmem,sz,tty,stat,time,cmd | grep test

#if 0
static void doDropPrivGid(int iGid)
{
    int res;
    uchar szBuf[1024];

    res = setgroups(0, NULL); /* remove all supplementary group IDs */
    if(res) {
        perror("could not remove supplemental group IDs");
        exit(1);
    }
    DBGPRINTF("setgroups(0, NULL): %d\n", res);
    res = setgid(iGid);
    if(res) {
        /* if we can not set the userid, this is fatal, so let's unconditionally abort */
        perror("could not set requested group id");
        exit(1);
    }
}
#endif

int main(int argc, const char *argv[])
{
    uid_t uid = 1004;    // HARDCODED bas
    if (setuid(uid) == -1) {
        perror("setuid");
        return -1;
    }

    // TODO setgroups(0, NULL);

    sleep(100);

    return 0;
}

