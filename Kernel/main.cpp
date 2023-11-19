#include "uart.cpp"
#include <stdio.h>
#include "klib.h"

extern "C" void kernel_main(void) {
    UART::init();
    kprintf("Hello World!\n");
    ()kj wkprintf("This is a test");

}
