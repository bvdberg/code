#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#define PRINTF_FORMAT_CHECK(format_idx, args_idx) __attribute__ ((__format__(printf, format_idx, args_idx)))

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

// optional call to log to syslog
void log_output_syslog(const char* name);

// optional call to log to file
void log_output_file(const char* filename);

void log_close();

void log_setLevel(LogLevel lvl);

#ifndef NDEBUG
void log_debug(const char* format, ...) PRINTF_FORMAT_CHECK(1, 2);
#else
#define log_debug(...) do {} while (0)
#endif

void log_info(const char* format, ...) PRINTF_FORMAT_CHECK(1, 2);
void log_warn(const char* format, ...) PRINTF_FORMAT_CHECK(1, 2);
void log_error(const char* format, ...) PRINTF_FORMAT_CHECK(1, 2);
void log_fatal(const char* format, ...) PRINTF_FORMAT_CHECK(1, 2);


#ifndef NDEBUG
void log_debug_buf(const char* text, const uint8_t* data, unsigned len);
#else
#define log_debug_buf(...) do {} while (0)
#endif

void log_info_buf(const char* text, const uint8_t* data, unsigned len);

#ifdef __cplusplus
}
#endif

#endif

