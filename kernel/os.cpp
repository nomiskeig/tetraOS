#include <kernel/os.h>
#include <kernel/UART.h>
#include <kernel/libk/kstdio.h>
#include <kernel/memory.h>
#include <kernel/drivers/virtio/blk.h>
#include <kernel/drivers/ext2.h>

extern "C" int os_start(void) {
    uart_init();
    set_log_level(LogLevel::PAGING | LogLevel::SYSTEM |
                  LogLevel::VIRTUAL_MEMORY | LogLevel::ERROR | LogLevel::WARNING | LogLevel::VIRTIO |LogLevel::FS);
    log(LogLevel::SYSTEM, "Booted to OS");
    physical_allocator_init();
    paging_init();
    virtual_allocator_init();
    char *first = (char *)kalloc(4);
    *first = 0x1;

    VirtIOBlockDevice* block_device = get_block_device();
    block_device->init();
    EXT2* ext2 = new  EXT2(block_device);
    ext2->read_file("testfile.txt");
    ext2->read_file("folder1/folder2/folder3/file.txt");

    return 0;
}
