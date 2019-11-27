#include "logger.h"

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>


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
        return "Fatal";
    case Error:
        return "Error";
    case Warn:
        return "Warn ";
    case Info:
        return "Info ";
    case Debug:
        return "Debug";
    }
    return "?";
}

static LogLevel level;
static bool useColor;
static bool relativeTimes;
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
    startTime = current_time();
}

void log_setLevel(LogLevel lvl) {
    level = lvl;
}

static void log_internal(LogLevel level_, const char* color_, const char* format, va_list args) {
    char buffer[256];
    char* cp = buffer;

    if (useColor) cp += sprintf(cp, "%s", color_);
    *cp++ = '[';
    if (relativeTimes) {
        uint64_t now = current_time();
        now -= startTime;
        uint64_t subsec = now % 1000000;
        cp += sprintf(cp, "%04ld.%06ld", now / 1000000, subsec);
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
    cp += vsprintf(cp, format, args);
    if (useColor) cp += sprintf(cp, "%s", ANSI_NORMAL);
    *cp++ = '\n';
    *cp = 0;
    printf("%s", buffer);
}

void log_debug(const char* format, ...) {
    if (Debug > level) return;
    va_list args;
    va_start(args, format);
    log_internal(Debug, ANSI_BLUE, format, args);
    va_end(args);
}

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

