#include <string.h>
#include "../tlibc.h" 
#include <unistd.h>

char *getcwd(char *buf, size_t size) {
    // TODO: pathname is also returned, but i do not know how to allocate that memory without leaking it...
    syscall(79, buf, size);
    return 0;
}
