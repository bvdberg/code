#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>

#include "CGroupUtil.h"

using namespace Luna;

int CGroupUtil::create(const char* controller, const char* path) {
    // check access

    // combine controller + path
    // mkdir_parents();

    // TMEP use newpath
    if (mkdir(path, 0755) < 0) {
        if (errno == EEXIST) return 0;
        return -errno;
    }
    return 1;
}

int CGroupUtil::attach(const char* controller, const char* path, pid_t pid) {
    // TODO path should be create path + "group.procs"
    // NOTE: don't create file
    int fd = open(path, O_WRONLY | O_CLOEXEC | O_NOCTTY);
    if (fd < 0) return -errno;

    char pidstr[16];
    sprintf(pidstr, "%d\n", getpid());
    ssize_t written = write(fd, pidstr, strlen(pidstr));
    if (written != (int)strlen(pidstr)) {
        fprintf(stderr, "error writing pidstr: %s\n", strerror(errno));
        // TODO close
        return -1;
    }
    close(fd);
    return 0;
}

