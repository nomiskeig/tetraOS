#include "include/libk/kstdio.h"
#include "include/os.h"
#include "UART.h"
#include "include/memory.h"

extern "C" int os_start(void) {




    uart_init();
    printf("Booted to OS\n");
    physical_allocator_init();
    paging_init();
    printf("is after init");
    virtual_allocator_init();

    while(1) {}
    return 0;
}

