#include <string.h>
#include "../tlibc.h"
#include <unistd.h>

ssize_t write(int fd, const void* buf, size_t count) {
    syscall(1, fd, buf, count);
    return 0;

}
