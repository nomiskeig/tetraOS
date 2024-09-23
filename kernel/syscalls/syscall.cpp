#include <kernel/processor.h>
#include <kernel/libk/kstdio.h>
#include <kernel/exception.h>

void handle_syscall(SyscallParameters syscall_params) {

    printf("got syscall a1 0x%x\n", syscall_params.a1);
    printf("got syscall a2 0x%x\n", syscall_params.a2);
    printf("got syscall a3 0x%x\n", syscall_params.a3);
    printf("got syscall number %i\n", syscall_params.a7);
    printf("print from userspace: %s", syscall_params.a2);
    while(true) {

    }
}
