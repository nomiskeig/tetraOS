#ifndef TLIBC_H
#define TLIBC_H
#include <stdint.h>
#include <string.h>

#define PAGE_SIZE 4096

void make_syscall(int number);
void load_argument(int register_number, int64_t value);
size_t get_heap_start();
void malloc_init();
#endif
