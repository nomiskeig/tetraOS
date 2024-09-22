#include <string.h>
#include "../tlibc.h"

ssize_t write(int fd, const void* buf, size_t count) {
    load_argument(1, fd);
    load_argument(2, (uint64_t)buf);
    load_argument(3, count);
    make_syscall(2);
    return 0;

}
