#ifndef KSTDIO_H
#define KSTDIO_H

#include <cstdarg>
#include <cstdint>
enum LogLevel {
    DEBUG = 1,
    PAGING = 2,
    VIRTUAL_MEMORY = 4,
    PHYSICAL_MEMORY = 8,
    SYSTEM = 16,

};
void printf(char const* fmt, ...);
void log(LogLevel level, char const* fmt, ...);
void set_log_level(uint64_t level);
void vprintf(char const* fmt, va_list args);


#endif
