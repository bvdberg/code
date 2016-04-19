#ifndef LOGGER_H
#define LOGGER_H

#include <stdlib.h>

extern const char* stopReason;

void log_open(const char* name, const char* version);

void fatal(const char* reason, int retcode=-EXIT_FAILURE);

void bug(const char* reason);

#endif

