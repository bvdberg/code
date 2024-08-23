#include "logger.h"

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>


#define MAX_PRINT_LEN 32

#define ANSI_BLACK    "\033[0;30m"
#define ANSI_RED      "\033[0;31m"
#define ANSI_GREEN    "\033[0;32m"
#define ANSI_YELLOW   "\033[0;33m"
#define ANSI_BLUE     "\033[0;34m"
#define ANSI_MAGENTA  "\033[0;35m"
#define ANSI_CYAN     "\033[0;36m"
#define ANSI_GREY     "\033[0;37m"
#define ANSI_DARKGREY "\033[01;30m"
#define ANSI_BRED     "\033[01;31m"
#define ANSI_BGREEN   "\033[01;32m"
#define ANSI_BYELLOW  "\033[01;33m"
#define ANSI_BBLUE    "\033[01;34m"
#define ANSI_BMAGENTA "\033[01;35m"
#define ANSI_BCYAN    "\033[01;36m"
#define ANSI_WHITE    "\033[01;37m"
#define ANSI_NORMAL   "\033[0m"
#define ANSI_CLEAR    "\033[2J"

static const char* level2str(LogLevel lvl) {
    switch (lvl) {
    case Fatal:
        return "fatal";
    case Error:
        return "error";
    case Warn:
        return "warn ";
    case Info:
        return "info ";
    case Debug:
        return "debug";
    }
    return "?";
}

static int level2prio(LogLevel lvl) {
    switch (lvl) {
    case Fatal:     return LOG_CRIT;
    case Error:     return LOG_ERR;
    case Warn:      return LOG_WARNING;
    case Info:      return LOG_INFO;
    case Debug:     return LOG_DEBUG;
    }
    return LOG_INFO;    // satifsy compiler
}

static LogLevel level;
static bool useColor;
static bool relativeTimes;
static bool useSyslog;
static bool useFile;
static int file_fd;
static uint64_t startTime;

static uint64_t current_time() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    uint64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += (now.tv_nsec / 1000);
    return now64;
}

void log_init(LogLevel level_, bool useColor_, bool relativeTimes_) {
    level = level_;
    useColor = useColor_;
    relativeTimes = relativeTimes_;
    useSyslog = false;
    useFile = false;
    startTime = current_time();
}

void log_output_syslog(const char* name) {
    useSyslog = true;
    useColor = false;
    openlog(name, 0, LOG_USER);
}

void log_output_file(const char* filename) {
    file_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0660);
    if (file_fd == -1) {
        log_fatal("log: cannot open %s: %s", filename, strerror(errno));
    }
    useFile = true;
    useColor = false;
}

void log_close() {
    if (useSyslog) closelog();
    if (useFile) close(file_fd);
}

void log_setLevel(LogLevel lvl) {
    level = lvl;
}

static void log_internal(LogLevel level_, const char* color_, const char* format, va_list args) {
    char buffer[256];
    char* cp = buffer;

    if (useColor) cp += sprintf(cp, "%s", color_);
    if (!useSyslog) {
        *cp++ = '[';
        if (relativeTimes) {
            uint64_t now = current_time();
            now -= startTime;
            uint64_t subsec = now % 1000000;
            cp += sprintf(cp, "%04" PRIu64 ".%06" PRIu64, now / 1000000, subsec);
        } else {
            struct timeval tv;
            gettimeofday(&tv, NULL);
            time_t now = tv.tv_sec;
            struct tm* now2 = localtime(&now);
            cp += sprintf(cp, "%02d/%02d/%02d %02d:%02d:%02d.%lu",
                          now2->tm_year % 100, now2->tm_mon + 1, now2->tm_mday,
                          now2->tm_hour, now2->tm_min, now2->tm_sec, tv.tv_usec / 1000);
        }
        *cp++ = ']';
        cp += sprintf(cp, " %s: ", level2str(level_));
    }
    cp += vsnprintf(cp, 200, format, args);
    if (useColor) cp += sprintf(cp, "%s", ANSI_NORMAL);

    if (useSyslog) {
       syslog(level2prio(level_), "%s", buffer);
    } else {
        *cp++ = '\n';
        *cp = 0;
        if (useFile) {
            ssize_t len = cp - buffer;
            ssize_t written = write(file_fd, buffer, len);
            if (written != len) {
                fprintf(stderr, "log: error writing file: %s\n", strerror(errno));
                // fall back to stdout/stderr
                close(file_fd);
                file_fd = -1;
                useFile = false;

            }
        } else {
            printf("%s", buffer);
        }
    }
}

#ifndef NDEBUG
void log_debug(const char* format, ...) {
    if (Debug > level) return;
    va_list args;
    va_start(args, format);
    log_internal(Debug, ANSI_BLUE, format, args);
    va_end(args);
}
#endif

void log_info(const char* format, ...) {
    if (Info > level) return;
    va_list args;
    va_start(args, format);
    log_internal(Info, ANSI_NORMAL, format, args);
    va_end(args);
}

void log_warn(const char* format, ...) {
    if (Warn > level) return;
    va_list args;
    va_start(args, format);
    log_internal(Warn, ANSI_YELLOW, format, args);
    va_end(args);
}

void log_error(const char* format, ...) {
    if (Error > level) return;
    va_list args;
    va_start(args, format);
    log_internal(Error, ANSI_RED, format, args);
    va_end(args);
}

void log_fatal(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log_internal(Fatal, ANSI_RED, format, args);
    va_end(args);
    exit(EXIT_FAILURE);
}

#ifndef NDEBUG
void log_debug_buf(const char* text, const uint8_t* data, unsigned len) {
    char hex[MAX_PRINT_LEN*3 + 1];
    char* cp = hex;
    unsigned print_len = len;
    if (len > MAX_PRINT_LEN) print_len = MAX_PRINT_LEN - 1;
    for (unsigned i=0; i<print_len; i++) {
        cp += sprintf(cp, " %02X", data[i]);
    }
    if (len != print_len) cp += sprintf(cp, " ..");
    *cp = 0;
    log_debug("%s (%u bytes) %s", text, len, hex);
}
#endif

void log_info_buf(const char* text, const uint8_t* data, unsigned len) {
    char hex[MAX_PRINT_LEN*3 + 1];
    char* cp = hex;
    unsigned print_len = len;
    if (len > MAX_PRINT_LEN) print_len = MAX_PRINT_LEN - 1;
    for (unsigned i=0; i<print_len; i++) {
        cp += sprintf(cp, " %02X", data[i]);
    }
    if (len != print_len) cp += sprintf(cp, " ..");
    *cp = 0;
    log_info("%s (%u bytes) %s", text, len, hex);
}

