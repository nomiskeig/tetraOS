#include <kernel/exception.h>
#include <kernel/libk/kstdio.h>
#include <kernel/processor.h>

void handle_syscall(SyscallParameters syscall_params) {
    log(LogLevel::SYSCALL, "Handling system call with parameter %i",
        syscall_params.a7);
    switch (syscall_params.a7) {
    case 0:
        read((void *)syscall_params.a2, syscall_params.a3);
        break;
    case 1:
        printf("%s", syscall_params.a2);
        break;
    }
}

