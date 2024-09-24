#include <string.h>
#include "../tlibc.h"
ssize_t read(int fd, void* buf, size_t nbyte) {
    asm("add a1, zero, %0" : : "r"(fd) :);
    asm("add a2, zero, %0" : : "r"(buf) :);
    asm("add a3, zero, %0" : : "r"(nbyte) :);
    make_syscall(0);
    return 0;
    
}
