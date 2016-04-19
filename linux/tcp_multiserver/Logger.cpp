#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "Logger.h"

const char* stopReason = "?";

static void log_close() {
    printf("-------- stopped (pid=%d), reason: %s --------", getpid(), stopReason);
    fflush(stdout);
}

void log_open(const char* name, const char* version) {
    printf("-------- starting (pid=%d, version=%s) --------", getpid(), version);
    fflush(stdout);
    if (atexit(log_close) != 0) fatal("atexit");
}

void fatal(const char* reason, int retcode) {
    stopReason = reason;
    const char* msg = "";
    if (errno != 0) msg = strerror(errno);
    fprintf(stderr, "FATAL: %s: %s\n", reason, msg);
    exit(retcode);
}

void bug(const char* reason) {
    fprintf(stderr, "BUG: %s\n", reason);
}

