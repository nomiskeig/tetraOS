#include "include/libk/printf.h"
#include "os.h"
#include "UART.h"

extern "C" int os_start(void) {




    printf("Booted to OS\n");
    uart_init();
    physical_allocator_init();
    void* first_frame = kalloc_frame();
    void* second_frame = kalloc_frame();
    void* third_frame = kalloc_frame();
    
    kfree_frame(second_frame);
    void* fourth_frame = kalloc_frame();

    while(1) {}
    return 0;
}

