#include <string.h>
#include "../tlibc.h"
#include <unistd.h>
ssize_t read(int fd, void* buf, size_t nbyte) {
    syscall(0, fd, buf, nbyte);
    return 0;
    
}
