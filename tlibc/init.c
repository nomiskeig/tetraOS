#include "./tlibc.h"

static size_t HEAP_START;

void initialize_standard_library() {
    asm("add %0, zero, a2;": "=r"(HEAP_START));


}


size_t get_heap_start() {
    return HEAP_START;

}
