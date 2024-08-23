#include "include/os.h"
#include "UART.h"
#include "include/libk/kstdio.h"
#include "include/memory.h"
#include "include/drivers/virtio/blk.h"

extern "C" int os_start(void) {
    uart_init();
    set_log_level(LogLevel::PAGING | LogLevel::SYSTEM |
                  LogLevel::VIRTUAL_MEMORY | LogLevel::ERROR);
    log(LogLevel::SYSTEM, "Booted to OS");
    physical_allocator_init();
    paging_init();
    virtual_allocator_init();
    char *first = (char *)kalloc(4);
    *first = 0x1;
    log(LogLevel::ERROR, "test");

    kalloc(sizeof(uint64_t));
    kalloc(32);
    VirtIOBlockDevice* block_device = get_block_device();
    block_device->init();
    block_device->read(0,0,0);
    printf("gets here\n");


    while (1) {
    }
    return 0;
}
