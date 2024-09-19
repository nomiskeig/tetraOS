#include <kernel/processor.h>
#include <kernel/libk/kstdio.h>

void handle_syscall() {
    int syscall_number = read_register(Register::a7);
    printf("got syscall number %i\n", syscall_number);
}
