#include "include/libk/kstdio.h"
#include "include/os.h"
#include "UART.h"
#include "include/memory.h"

extern "C" int os_start(void) {
    uart_init();
    set_log_level(LogLevel::PAGING |LogLevel::SYSTEM| LogLevel::VIRTUAL_MEMORY);
    log(LogLevel::SYSTEM, "Booted to OS");
    physical_allocator_init();
    paging_init();
    virtual_allocator_init();

    while(1) {}
    return 0;
}

