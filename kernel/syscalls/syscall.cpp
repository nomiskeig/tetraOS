#include <kernel/processor.h>
#include <kernel/libk/kstdio.h>
#include <kernel/exception.h>

void handle_syscall(SyscallParameters syscall_params) {
    log(LogLevel::SYSCALL, "Handling system call with parameter %i",syscall_params.a7 );
    printf("%s", syscall_params.a2);
}

