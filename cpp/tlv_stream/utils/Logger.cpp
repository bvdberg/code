#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "utils/Logger.h"
#include "utils/color.h"

#define LONG_NAMES

static uint64_t getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, 0);
    uint64_t tv64 = tv.tv_sec;
    tv64 *= 1000000;
    tv64 += tv.tv_usec;
    return tv64;
}

const char* LogBackEnd::lvl2str(LogLevel lvl) {
    switch (lvl) {
#ifdef LONG_NAMES
    case LOG_BUG:   return "BUG  ";
    case LOG_FATAL: return "FATAL";
    case LOG_ERROR: return "ERROR";
    case LOG_WARN:  return "WARN ";
    case LOG_INFO:  return "INFO ";
    case LOG_DEBUG: return "DEBUG";
#else
    case LOG_BUG:   return "B";
    case LOG_FATAL: return "F";
    case LOG_ERROR: return "E";
    case LOG_WARN:  return "W";
    case LOG_INFO:  return "I";
    case LOG_DEBUG: return "D";
#endif
    }
    return 0;
}

Logger::Logger(const char* name_, LogBackEnd& backend_)
    : name(name_)
    , backend(backend_)
    , level(backend.defaultLevel)
{
    assert(name.size() < 16);
}

void Logger::debug(const char* format, ...) {
    if (LogBackEnd::LOG_DEBUG > level) return;
    va_list myargs;
    va_start(myargs, format);
    log_internal(LogBackEnd::LOG_DEBUG, ANSI_BLUE, format, myargs);
    va_end(myargs);
}

void Logger::info(const char* format, ...) {
    if (LogBackEnd::LOG_INFO > level) return;
    va_list myargs;
    va_start(myargs, format);
    log_internal(LogBackEnd::LOG_INFO, ANSI_WHITE, format, myargs);
    va_end(myargs);
}

void Logger::warn(const char* format, ...) {
    if (LogBackEnd::LOG_WARN > level) return;
    va_list myargs;
    va_start(myargs, format);
    log_internal(LogBackEnd::LOG_WARN, ANSI_YELLOW, format, myargs);
    va_end(myargs);
}

void Logger::error(const char* format, ...) {
    if (LogBackEnd::LOG_ERROR > level) return;
    va_list myargs;
    va_start(myargs, format);
    log_internal(LogBackEnd::LOG_ERROR, ANSI_RED, format, myargs);
    va_end(myargs);
}

void Logger::fatal(const char* format, ...) {
    va_list myargs;
    va_start(myargs, format);
    log_internal(LogBackEnd::LOG_FATAL, ANSI_RED, format, myargs);
    va_end(myargs);
    exit(-1);
}

void Logger::bug(const char* format, ...) {
    if (LogBackEnd::LOG_BUG > level) return;
    va_list myargs;
    va_start(myargs, format);
    log_internal(LogBackEnd::LOG_BUG, ANSI_RED, format, myargs);
    va_end(myargs);
    exit(-1);
}

void Logger::log_internal(LogBackEnd::LogLevel lvl, const char* color, const char* format, va_list args) {
    char buffer[256];
    char* cp = buffer;

    // TODO check size
    if (backend.useColor) cp += sprintf(cp, "%s", color);
    *cp++ = '[';
    if (backend.relativeTimes) {
        uint64_t now = getCurrentTime();
        now -= backend.startTime;
        cp +=  sprintf(cp, "%04" PRIu64, now / 1000000lu);
        uint64_t subsec = now % 1000000lu;
        cp += sprintf(cp, ".%06" PRIu64, subsec);
    } else {
        struct timeval tv;
        gettimeofday(&tv, 0);
        time_t now = tv.tv_sec;
        struct tm* now2 = localtime(&now);
        cp += sprintf(cp, "%02d/%02d/%02d %02d:%02d:%02d.%03lu",
                      now2->tm_year % 100, now2->tm_mon + 1, now2->tm_mday, now2->tm_hour, now2->tm_min, now2->tm_sec, tv.tv_usec / 1000);
    }
    *cp++ = ']';
    cp += sprintf(cp, " %s %s: ", LogBackEnd::lvl2str(lvl), name.c_str());
    cp += vsprintf(cp, format, args);

    if (backend.useColor) cp += sprintf(cp, ANSI_NORMAL);
    *cp++ = '\n';
    *cp = 0;
    backend.write(buffer);
}


StdoutLogger::StdoutLogger(LogBackEnd::LogLevel lvl, bool relativeTimes_)
    : LogBackEnd(lvl, true, relativeTimes_)
{
    useColor = isatty(1);
    if (relativeTimes_) startTime = getCurrentTime();
}

StdoutLogger::~StdoutLogger() {
    fflush(stdout);
}

void StdoutLogger::write(const char* data) {
    ssize_t written = ::write(1, data, strlen(data));
    (void)written;
}

