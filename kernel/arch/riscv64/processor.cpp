#include <kernel/processor.h>

register64_t read_register(Register reg) {
    register64_t val;
    switch (reg) {
    case a1:
        asm("add %0, a1, zero;" : "=r"(val));
        break;

    case a2:
    case a3:
    case a4:
    case a5:
    case a6:
        break;
    case a7:
        asm("add %0, a7, zero;" : "=r"(val));

        break;
    }
    return val;
}
