#ifndef PAGE_H
#define PAGE_H

#include <cstddef>
void page_init();
void *alloc(size_t amount);
void dealloc(void *address);
void print_allocations();

#endif

