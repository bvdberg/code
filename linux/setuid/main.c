#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <grp.h>


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

static void createFile(const char* name) {
    int fd = open(name, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        fprintf(stderr, "Error creating file %s: %s\n", name, strerror(errno));
        exit(-1);
    }
    const char* data = "TESTING\n";
    ssize_t numwritten = write(fd, data, strlen(data));
    if (numwritten != (int)strlen(data)) {
        perror("write");
    }
    close(fd);
}

static void createChild(const char* name) {
    pid_t child_pid = fork();
    if (child_pid == 0) {   // child
        printf("CHILD: %s\n", name);
        sleep(100);
        exit(-1);
    } else {    // parent (or -1)
        printf("Child = %d\n", child_pid);
    }
}

static void changeUser(uid_t uid, gid_t gid) {
#if 0
    // TODO
    // remove all supplementary group IDs
    if (setgroups(0, NULL) == -1) {
        perror("could not remove supplemental group IDs");
        exit(-1);
    }
#endif
    // change group
    if (setgid(gid) == -1) {
        perror("could not set requested group id");
        exit(-1);
    }
    // change user
    if (setuid(uid) == -1) {
        perror("setuid");
        exit(-1);
    }
}

int main(int argc, const char *argv[])
{
    createChild("child1");
    createFile("file1.txt");

    changeUser(1004, 1004);   // hardcoded BAS

    // TODO setgroups(0, NULL);
    createChild("child2");
    createFile("file2.txt");

    sleep(100);

    return 0;
}

