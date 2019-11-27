#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
    Fatal = 0,
    Error,
    Warn,
    Info,
    Debug,
} LogLevel;

void log_init(LogLevel level_, bool useColor_, bool relativeTimes_);

void log_setLevel(LogLevel lvl);

void log_debug(const char* format, ...);
void log_info(const char* format, ...);
void log_warn(const char* format, ...);
void log_error(const char* format, ...);
void log_fatal(const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif

