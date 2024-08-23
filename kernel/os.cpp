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

    VirtIOBlockDevice* block_device = get_block_device();
    block_device->init();
    char* bytes = (char*)kalloc(512);
    //*(uint32_t*)bytes = 0xDEADBEEF;
    block_device->read(1, 32, bytes);
    
    for (size_t i = 0; i < 512 / 8; i++) {
        printf("data at 0x%x: 0x%x\n", (uint64_t *)bytes + i,
               *((uint64_t *)(bytes) + i));
    }
    printf("gets here\n");


    while (1) {
    }
    return 0;
}
