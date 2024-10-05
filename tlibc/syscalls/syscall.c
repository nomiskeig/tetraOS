#include "../tlibc.h"
#include <stdarg.h>
#include <stdio.h>

typedef struct Registers {
    long gp;
} Registers;
long syscall(long number, ...) {
    Registers regs;
    //asm volatile("add %0, gp, zero;" : "=r"(regs.gp));
    
    // TODO: return result
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
    long return_value;
    asm volatile("add %0, a0, zero;" : "=r"(return_value));
    //asm("add gp, zero, %0" : : "r"(regs.gp) :);
    // TODO restore CPU registers

     return return_value;
    //return 0;
}
