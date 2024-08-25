#ifndef UART_H
#define UART_H
#include <tlib/stdint.h>


#define UART_ADDR 0x10000000
#define UART_THR (uint8_t *)(UART_ADDR + 0x0) // Transmitter holding register
#define UART_LSR (uint8_t*)(UART_ADDR + 0x5) // LSR: line status register
#define UART_LSR_EMPTY_MASK 0x40

void uart_init();
void put_char(const char c);

#endif
