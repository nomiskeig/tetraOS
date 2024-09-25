#include <string.h>
#include "../tlibc.h" 
char *getcwd(char *buf, size_t size) {
    asm("add a1, zero, %0" : : "r"(buf) :);
    asm("add a2, zero, %0" : : "r"(size) :);
    make_syscall(79);
    // TODO: pathname is also returned, but i do not know how to allocate that memory without leaking it...
    return 0;
}
