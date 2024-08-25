
#ifndef OS_H
#define OS_H

#include <tlib/stdint.h> 
// returns a pointer to virtual memory of size bytes
void* kmalloc(uint32_t size);

#endif
