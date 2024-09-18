#include <string.h>

int write2() {
    asm("ecall");
}
ssize_t write(int fd, const void* buf, size_t count) {
    asm("ecall");

}
