#include <string.h>
#include "../tlibc.h"

ssize_t write(int fd, const void* buf, size_t count) {
    asm("add a1, zero, %0" : : "r"(fd) :);
    asm("add a2, zero, %0" : : "r"(buf) :);
    asm("add a3, zero, %0" : : "r"(count) :);
    make_syscall(1);
    return 0;

}
