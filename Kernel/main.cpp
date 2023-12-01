#include "klib.h"
#include "uart.cpp"
#include "page.h"

extern "C" void kernel_main(void) {
  UART::init();
  page_init();
  void *mem = alloc(2);
  kprintf("Memory starts at: %x\n", mem);
  void *mem2 = alloc(2);
  kprintf("Memory starts at: %x\n", mem2);
  void *mem3= alloc(2);
  kprintf("Memory starts at: %x\n", mem3);
  void *mem4 = alloc(2);
  kprintf("Memory starts at: %x\n", mem4);
  print_allocations();
  dealloc(mem2);
  print_allocations();
}
