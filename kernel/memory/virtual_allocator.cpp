
#include "../include/libk/kstdio.h"
#include "../include/libk/list.h"
#include "../include/memory.h"
extern uint64_t VIRTUAL_HEAP_START;
extern uint64_t VIRTUAL_HEAP_END;

static AllocList *alloc_list;

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
    //
    printf("Address of the frame:           0x%x\n", frame.get_address());
    printf("Address of the virtual page:    0x%x\n", page.get_address());
    // Initialise the first element of the list
    alloc_list = ((AllocList *)page.get_address());
    AllocItem *first_item = &(AllocItem)(*(char *)page.get_address() + sizeof(AllocList));
    first_item->set_start_address((void*)((char*)&first_item + sizeof(AllocItem)));
    //first_item->set_size(VIRTUAL_HEAP_END - (uint64_t)*(char*)first_item->get_start_address());
    printf("gets here");
    printf("Address of first free item: 0x%x\n", first_item->get_start_address());
    printf("Size of the first free item: 0x%x\n", first_item->get_size());
}
