
#include "../include/libk/kstdio.h"
#include "../include/libk/list.h"
#include "../include/memory.h"
extern uint64_t VIRTUAL_HEAP_START;
// Everthing with the VPN[3] = 111111111 is recursivley mapped to the page table so we cannot use that for kernel memory
#define VIRTUAL_HEAP_END 0xFFFFFFE000000000

static AllocItem *alloc_list;

void virtual_allocator_init() {
    printf("Initialising virtual allocator\n");
    // this is the first frame on which we store our managing list
    // the list keeps track of the holes in the virtual address space
    // PhysicalFrame physical_address = (PhysicalFrame)(uint64_t*)
    // kalloc_frame(); VirtualPage virtual_page = alloc_list = (AllocList
    // *)physical_address;
    PhysicalFrame frame{kalloc_frame()};
    VirtualPage page{frame.get_virtual_address()};
    // we have to operate on the virtual page
    
    map_page(&page, &frame);
    printf("Address of the frame:           0x%x\n", frame.get_address());
    printf("Address of the virtual page:    0x%x\n", page.get_address());
    AllocItem *first_item = (AllocItem*)(page.get_address());
    first_item->set_start_address((void*)(first_item + 1));
    first_item->set_size(PAGE_SIZE - sizeof(AllocItem));

    printf("Address of first free item: 0x%x\n", first_item->get_start_address());
    printf("Size of the first free item: 0x%x\n", first_item->get_size());
}


void kmalloc() {
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

}
