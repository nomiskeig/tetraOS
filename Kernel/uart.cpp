#include <stdint.h>

namespace UART {
    #define UART_BASE 0x10000000 
    void init() {
        uint8_t* base = (uint8_t*)UART_BASE;
        // set word length to 8 bits
        *(volatile uint8_t*)(base + 0x3) = 0b11;
        // enable FIFO
        *(volatile uint8_t*)(base + 0x2) = 0b1;
        // enbalbe receiver buffer interrupts
        *(volatile uint8_t*)(base + 0x1) = 0b1;

        uint16_t divisor = 592;
        uint8_t divisor_lsb = divisor & 0xFF;
        uint8_t divisor_msb = (divisor >> 8);
        *(volatile uint8_t*)(base + 0x3) = 0b10000011;
        *(volatile uint8_t*)(base) = divisor_lsb;
        *(volatile uint8_t*)(base + 0x1) = divisor_msb;
        *(volatile uint8_t*)(base + 0x3) = 0b11; 

    }

};
    void write_char(uint8_t c) {
        uint8_t* base = (uint8_t*)UART_BASE;
        *(volatile uint8_t*)(base) = c;
    }



