#include <kernel/libk/kstdio.h>
#include <kernel/syscalls/syscall.h>
#include <kernel/exception.h>

extern "C" void jump_back_to_userspace(long);
/*
 * Handles exceptions which trap in to the machine mode handler. We get here from jump_to_machine_exception_handler in boot.s and should be in supervisor mode.
    */
extern "C" int machine_exception_handler(void) {
    // put the syscall parameters onto the stack so that we do not have to worry about overwriting it
    SyscallParameters syscal_params;
    asm volatile("add %0, a1, zero;" : "=r"(syscal_params.a1));
    asm volatile("add %0, a2, zero;" : "=r"(syscal_params.a2));
    asm volatile("add %0, a3, zero;" : "=r"(syscal_params.a3));
    asm volatile("add %0, a4, zero;" : "=r"(syscal_params.a4));
    asm volatile("add %0, a5, zero;" : "=r"(syscal_params.a5));
    asm volatile("add %0, a6, zero;" : "=r"(syscal_params.a6));
    asm volatile("add %0, a7, zero;" : "=r"(syscal_params.a7));
    uint64_t cause = 0x1;
    uint64_t mtval;
    // store the value we come from from userspace (this is used in the case of syscalls to return afterwards)
    uint64_t mepc_value;
    asm volatile("add %0, t0, zero;" : "=r"(cause));
    asm volatile("add %0, t1, zero;" : "=r"(mtval));
    asm volatile("add %0, t2, zero;" : "=r"(mepc_value));

    //  store the registesr a1
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
    case 0x8:
        handle_syscall(syscal_params);
        jump_back_to_userspace(mepc_value);
        break;
    case 0xC:
        log(LogLevel::EXCEPTION, "Instruction page fault at 0x%x", mtval);
        break;
    case 0xD:
        log(LogLevel::EXCEPTION, "Load page fault at 0x%x", mtval);
        break;
    case 0xF:
        log(LogLevel::EXCEPTION, "Store/AMO page fault at 0x%x", mtval);
        break;
    default: 
        log(LogLevel::EXCEPTION, "Got an unknown exception: %i", cause);
        
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
