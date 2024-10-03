#ifndef TLIBC_H
#define TLIBC_H
#include <stdint.h>
#include <string.h>

#define PAGE_SIZE 4096

size_t get_heap_start();
void malloc_init();
void init_printf();
#endif
