#ifndef CGROUP_UTIL_H
#define CGROUP_UTIL_H

/*
    This code is based on the systemd code in src/shared/cgroup-util.c/h,
*/

namespace Luna {

class CGroupUtil {
public:
    static int create(const char* controller, const char* path);
    static int attach(const char* controller, const char* path, pid_t pid);
};

}

#endif

