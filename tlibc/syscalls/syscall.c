#include "../tlibc.h"
#include <stdarg.h>


long syscall(long number, ...) {
    // TODO: save CPU registers

    va_list args;
    va_start(args, number);

    asm("add a0, zero, a1" : : :);
    asm("add a1, zero, a2" : : :);
    asm("add a2, zero, a3" : : :);
    asm("add a3, zero, a4" : : :);
    asm("add a4, zero, a5" : : :);
    asm("add a5, zero, a6" : : :);
    asm("add a7, zero, %0" : : "r"(number) :);
    asm("ecall");
    // TODO restore CPU registers
}
