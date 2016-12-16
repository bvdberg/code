#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>
#include <stdarg.h>
#include <string>

class LogBackEnd {
public:
    enum LogLevel {
        LOG_FATAL=0,
        LOG_BUG,
        LOG_ERROR,
        LOG_WARN,
        LOG_INFO,
        LOG_DEBUG,
    };
    static inline const char* lvl2str(LogLevel lvl);

    LogBackEnd(LogLevel lvl, bool color, bool relativeTimes_)
        : defaultLevel(lvl), useColor(color), relativeTimes(relativeTimes_), startTime(0)
    {}
    virtual ~LogBackEnd () {}

    virtual void write(const char* data) = 0;

    void setDefaultLevel(LogLevel l) { defaultLevel = l; }

    LogLevel defaultLevel;
    bool useColor;
    bool relativeTimes;
    uint64_t startTime;
};

class StdoutLogger : public LogBackEnd {
public:
    StdoutLogger(LogLevel lvl, bool relativeTimes_ = true);
    virtual ~StdoutLogger();

    virtual void write(const char* data);
private:
    StdoutLogger(const StdoutLogger&);
    StdoutLogger& operator=(const StdoutLogger&);
};

#define PRINTF_FORMAT_CHECK(format_index, args_index) __attribute__ ((__format__(printf, format_index, args_index)))
class Logger {
public:
    Logger(const char* name_, LogBackEnd& backend_);
    ~Logger() {}

    void setLevel(LogBackEnd::LogLevel lvl) { level = lvl; }
    LogBackEnd& getBackend() const { return backend; }

    void debug(const char* format, ...) PRINTF_FORMAT_CHECK(2, 3);
    void info(const char* format,  ...) PRINTF_FORMAT_CHECK(2, 3);
    void warn(const char* format,  ...) PRINTF_FORMAT_CHECK(2, 3);
    void error(const char* format, ...) PRINTF_FORMAT_CHECK(2, 3);
    void fatal(const char* format, ...) PRINTF_FORMAT_CHECK(2, 3);
    void bug(const char* format,   ...) PRINTF_FORMAT_CHECK(2, 3);
private:
    void log_internal(LogBackEnd::LogLevel lvl, const char* color, const char* format, va_list args);

    std::string name;
    LogBackEnd& backend;
    LogBackEnd::LogLevel level;

    Logger(const Logger&);
    Logger& operator=(const Logger&);
};

#endif

