
#include "../include/libk/kstdio.h"
#include "../include/libk/list.h"
#include "../include/memory.h"
extern uint64_t VIRTUAL_HEAP_START;
// Everthing with the VPN[3] = 111111111 is recursivley mapped to the page table so we cannot use that for kernel memory

static FreeSpace *free_list;

void virtual_allocator_init() {
    log(LogLevel::VIRTUAL_MEMORY, "Initialising virtual allocator");
    // this is the first frame on which we store our managing list
    // the list keeps track of the holes in the virtual address space
    // PhysicalFrame physical_address = (PhysicalFrame)(uint64_t*)
    // kalloc_frame(); VirtualPage virtual_page = alloc_list = (AllocList
    // *)physical_address;
    PhysicalFrame* frame = (PhysicalFrame*)kalloc_frame();
    VirtualPage* page = (VirtualPage*)VIRTUAL_HEAP_START;
    // we have to operate on the virtual page
    
    map_page(page, frame);
    FreeSpace *first_item = (FreeSpace*)(page->get_address());
    first_item->set_start_address((void*)(first_item + 1));
    first_item->set_size(PAGE_SIZE - sizeof(FreeSpace));
    free_list = first_item;

    log(LogLevel::VIRTUAL_MEMORY, "Initialization of virtual allocator done");
}


void* kmalloc(uint64_t size) {
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

    FreeSpace * current = free_list;
    // Search list for enough memory
    while (current->get_size() < size) {
        current = current->get_next();
    }
    

}
