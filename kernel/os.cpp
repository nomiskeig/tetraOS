#include <kernel/UART.h>
#include <kernel/drivers/ext2.h>
#include <kernel/drivers/virtio/blk.h>
#include <kernel/libk/kstdio.h>
#include <kernel/memory.h>
#include <kernel/drivers/ext2.h>
#include <kernel/os.h>

extern "C" int os_start(void) {
    uart_init();
    set_log_level(   LogLevel::SYSTEM | LogLevel::ERROR |
                  LogLevel::WARNING | LogLevel::VIRTIO | LogLevel::FS);
    log(LogLevel::SYSTEM, "Booted to OS");
    physical_allocator_init();
    paging_init();
    virtual_allocator_init();

    VirtIOBlockDevice *block_device = get_block_device();
    if (block_device == 0x0) {
        log(LogLevel::ERROR, "Did not find a block device.");
        return -1;
    }
    block_device->init();
    EXT2 *ext2 = new EXT2(block_device);
    size_t testfile_size = get_file_size("testfile.txt");
    char* buffer1 = (char*)kalloc(testfile_size);
    read_file("testfile.txt", testfile_size, buffer1);
    for (size_t i = 0; i < testfile_size; i++) {
        printf("%c", *(buffer1 + i));
    }

    return 0;
}
