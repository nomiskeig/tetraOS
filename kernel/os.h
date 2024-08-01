
#ifndef OS_H
#define OS_H

#include <cstdint>
void physical_allocator_init();

void* kalloc_frame();
void kfree_frame(void*);

#endif
