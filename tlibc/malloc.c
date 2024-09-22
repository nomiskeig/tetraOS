#include <stdint.h>
#include <string.h>
#include <unistd.h>

static uint64_t HEAP_START;
static uint64_t CURRENT_HEAP_END;
void malloc_init() {
    // this is the first frame on which we store our managing list
    // the list keeps track of the holes in the virtual address space
    // PhysicalFrame physical_address = (PhysicalFrame)(uint64_t*)
    // kalloc_frame(); VirtualPage virtual_page = alloc_list = (AllocList
    // *)physical_address;
    HEAP_START = (uint64_t) sbrk(0);
}

void *malloc(size_t size) {
    /*
     * Steps:
     * 1. Search the list for existing space
     * 2. If space is found:
     * 3.1. Adjust the list
     * 3.2. Put size of allocation and magic string at the beginning (header)
     * 5. Return pointer with offset of header
     * 6. If no space is found:
     * Allocate new phyiscal page and map it
     *
     */
    return (void*)HEAP_START;

}
