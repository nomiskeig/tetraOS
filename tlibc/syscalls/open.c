#include <unistd.h>
#include <sys/types.h>

int open(const char* pathname, int flags, mode_t mode) {
    return syscall(2, pathname, flags, mode);
}
