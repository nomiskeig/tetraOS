#ifndef KSTDIO_H
#define KSTDIO_H

#include <tlib/stdarg.h>
#include <tlib/stdint.h>
#include <tlib/stddef.h>
enum LogLevel {
    DEBUG = 1,
    PAGING = 2,
    VIRTUAL_MEMORY = 4,
    PHYSICAL_MEMORY = 8,
    SYSTEM = 16,
    ERROR = 32,
    VIRTIO = 64,
    WARNING = 128,
    FS = 256,
    PROCESS = 512,
    EXCEPTION = 1024,
    SYSCALL = 2048

};
void printf(char const *fmt, ...);
size_t read(void* buffer, size_t size);
void log(LogLevel level, char const *fmt, ...);
void set_log_level(uint64_t level);
void vprintf(char const *fmt, va_list args);

#endif
