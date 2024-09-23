#include <unistd.h>
#include "../tlibc.h"
void *sbrk(intptr_t increment)  {
    // TODO: implement this properly
    return (void*)get_heap_start();
}
