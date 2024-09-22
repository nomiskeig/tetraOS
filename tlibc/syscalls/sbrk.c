#include <unistd.h>
#include "../tlibc.h"
void *sbrk(intptr_t increment)  {
    return (void*)get_heap_start();
}
