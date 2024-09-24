#include "UART.h"
#define UART 0xFFFF800000000000 + 0x10000000L
#define UART_THR                                                               \
    (volatile uint8_t *)(UART + 0x00) // THR:transmitter holding register
#define UART_RHR                                                               \
    (volatile uint8_t *)(UART + 0x00) // RHR:Receive holding register
#define UART_DLL                                                               \
    (volatile uint8_t *)(UART + 0x00) // LSB of Divisor Latch (write mode)
#define UART_DLM                                                               \
    (volatile uint8_t *)(UART + 0x01) // MSB of Divisor Latch (write mode)
#define UART_IER (volatile uint8_t *)(UART + 0x01) // Interrupt Enable Register
#define UART_LCR (volatile uint8_t *)(UART + 0x03) // Line Control Register
#define UART_LSR (volatile uint8_t *)(UART + 0x05) // LSR:line status register
#define UART_LSR_EMPTY_MASK                                                    \
    0x40 // LSR Bit 6: Transmitter empty; both the THR and LSR are empty
#define UART_LSR_RX_READY 0x01  
#define UART_REGR(reg) (*(reg))
#define UART_REGW(reg, v) ((*reg) = (v))
void uart_init() {
    /* disable interrupts */
    UART_REGW(UART_IER, 0x00);

    /* Baud rate setting */
    uint8_t lcr = UART_REGR(UART_LCR);
    UART_REGW(UART_LCR, lcr | (1 << 7));
    UART_REGW(UART_DLL, 0x03);
    UART_REGW(UART_DLM, 0x00);

    lcr = 0;
    UART_REGW(UART_LCR, lcr | (3 << 0));

    uint8_t ier = UART_REGR(UART_IER);
    UART_REGW(UART_IER, ier | (1 << 0));
}
void put_char(char c) {
    while ((*UART_LSR & UART_LSR_EMPTY_MASK) == 0)
        ;
    *UART_THR = c;
}

char get_char() {
    while ((*UART_LSR & UART_LSR_RX_READY) == 0) {
        ;
    }
    char c = UART_REGR(UART_RHR);
    return c;

}
