#include <kernel/libk/kstdio.h> 
#include <tlib/stdarg.h>

static uint64_t logLevel = 0;
void log(LogLevel level, char const *fmt, ...) {
    if ((logLevel & level) == 0) {
        return;
    }
    va_list args;
    va_start(args, fmt);
    const char *prefix = "";
    switch (level) {
    case LogLevel::DEBUG:
        prefix = "[DEBUG]";
        break;
    case LogLevel::PAGING:
        prefix = "[PAGING]";
        break;
    case LogLevel::VIRTUAL_MEMORY:
        prefix = "[VMEM]";
        break;
    case LogLevel::PHYSICAL_MEMORY:
        prefix = "[PMEM]";
        break;
    case LogLevel::SYSTEM:
        prefix = "[SYSTEM]";
        break;
    case LogLevel::ERROR:
        prefix = "\u001b[31m[ERROR]\u001b[37m";
        break;
    case LogLevel::VIRTIO:
        prefix = "[VIRTIO]";
        break;
    case LogLevel::WARNING:
        prefix = "\u001b[33m[WARN]\u001b[37m";
        break;
    case LogLevel::FS:
        prefix= "[FS]";
        break;
    case LogLevel::PROCESS:
        prefix= "[PROCESS]";
        break;
    case LogLevel::EXCEPTION:
        prefix = "\u001b[31m[EXCEPTION]\u001b[37m";
        break;
    case LogLevel::SYSCALL:
        prefix= "[SYSCALL]";
        break;
    }
    printf(prefix);
    printf(" ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

void set_log_level(uint64_t level) { logLevel = level; }
