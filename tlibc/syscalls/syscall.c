#include "../tlibc.h"

void make_syscall(int number) {
  asm("add a7, zero, %0" : : "r"(number) :);
  asm("ecall");
}

/*
 * Loads value into the register. Works for registers a1 through a6
 */
void load_argument(int register_number, int64_t value) {
  switch (register_number) {
  case 1:
    asm("add a1, zero, %0" : : "r"(value) :);
    break;
  case 2:
    asm("add a2, zero, %0" : : "r"(value) :);
    break;
  case 3:
    asm("add a3, zero, %0" : : "r"(value) :);
    break;
  }
  return;
}
