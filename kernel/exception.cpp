#include <kernel/libk/kstdio.h>

extern "C" int machine_exception_handler(void) {

    printf("is in machine exception handler\n");
    uint64_t cause = 0x1;
    uint64_t mtval;
    asm volatile("add %0, t0, zero;" : "=r"(cause));
    asm volatile("add %0, t1, zero;" : "=r"(mtval));
    printf("cause: 0x%x\n", cause);
    switch (cause) {
    case 0x0:
        log(LogLevel::EXCEPTION, "Instruction address misaligned");
        break;
    case 0x1:
        log(LogLevel::EXCEPTION, "Instruction access fault");
        break;
    case 0x2:
        log(LogLevel::EXCEPTION, "Illegal instruction");
        break;

    case 0xC:
        log(LogLevel::EXCEPTION, "Load page fault at 0x%x", mtval);
        break;
    case 0xF:
        log(LogLevel::EXCEPTION, "Store/AMO page fault at 0x%x", mtval);
        break;
    }
    // we do not handle exceptions gracefully, we just die lol
    while (true) {
    }
}

extern "C" int supervisor_exception_handler(void) {
    printf("is in supervisor exception handler\n");
    while (true) {
    }
}
