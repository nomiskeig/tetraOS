
#include "../include/libk/kstdio.h"
#include "../include/libk/list.h"
#include "../include/memory.h"
#include <cstdint>
extern uint64_t KERNEL_END;

static AllocList *alloc_list;

void virtual_allocator_init() {
    printf("Initialising virtual allocator\n");
    // this is the first frame on which we store our managing list
    // the list keeps track of the holes in the virtual address space
    // PhysicalFrame physical_address = (PhysicalFrame)(uint64_t*)
    // kalloc_frame(); VirtualPage virtual_page = alloc_list = (AllocList
    // *)physical_address;
    PhysicalFrame frame{kalloc_frame()};
    VirtualPage virtual_address{frame.get_virtual_address()};
    // we have to operate on the virtual page
    //
    printf("Address of the frame:           0x%x\n", frame.get_address());
    printf("Address of the virtual page:    0x%x\n", virtual_address.get_address());
}
