#include "../tlibc.h"

void make_syscall(int number) {
    asm("add a7, zero, %0" : : "r"(number) : );
    asm("ecall");
}

/*
 * Loads value into the register. Works for registers a1 through a6
*/
void load_argument(int register_number, int64_t value) {


}
