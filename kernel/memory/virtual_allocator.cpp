
#include "../include/libk/kstdio.h"
#include "../include/libk/list.h"
#include "../include/memory.h"
extern uint64_t VIRTUAL_HEAP_START;

static FreeSpace *free_list;
static uint64_t virtual_alloc_end;

void *operator new(size_t size) {
    return kalloc(size);
}
void operator delete(void *p)  {
    log(LogLevel::ERROR, "Unimplemented call to delete operator");
    //kfree(p);
}
extern "C" void *memset(void* s, int c, size_t n) {
    char * i  =(char*)s;
    uint64_t counter = 0;
    while (counter < n) {
        *i = c;
        i++;
        counter++;

    }
    return s;

}extern "C" void* memcpy(void *dest, const void *src, size_t len) {
    char* dest_char = (char*)dest;
    char* src_char = (char*)src;
    size_t current = 0;
    while (current < len) {
        dest_char[current] = src_char[current];
        current++;
    }
    return dest;

}

void virtual_allocator_init() {
    log(LogLevel::VIRTUAL_MEMORY, "Initialising virtual allocator");
    // this is the first frame on which we store our managing list
    // the list keeps track of the holes in the virtual address space
    // PhysicalFrame physical_address = (PhysicalFrame)(uint64_t*)
    // kalloc_frame(); VirtualPage virtual_page = alloc_list = (AllocList
    // *)physical_address;
    PhysicalFrame *frame = (PhysicalFrame *)kalloc_frame();
    VirtualPage *page = (VirtualPage *)VIRTUAL_HEAP_START;
    // we have to operate on the virtual page

    map_page(page, frame);
    FreeSpace *first_item = (FreeSpace *)(page->get_address());
    first_item->set_size(PAGE_SIZE);
    printf("first item size: 0x%x\n", first_item->get_size());
    first_item->set_last();
    free_list = first_item;
    virtual_alloc_end = VIRTUAL_HEAP_START + PAGE_SIZE;

    log(LogLevel::VIRTUAL_MEMORY, "Virtual alloc end: 0x%x\n", virtual_alloc_end);
    log(LogLevel::VIRTUAL_MEMORY, "Initialization of virtual allocator done");
}

void *kalloc(uint64_t size) {
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

    FreeSpace *current = free_list;
    bool is_first = true;
    // Search list for enough memory
    while (!current->is_last() && current->get_size() < size) {
        is_first = false;
        current = current->get_next();
    };
    if (current->is_last() && current->get_size() < size) {
        while (current->get_size() < size) {

        PhysicalFrame* new_frame = (PhysicalFrame*)kalloc_frame();
        map_page((VirtualPage*)virtual_alloc_end, new_frame);
        current->set_size(current->get_size() + PAGE_SIZE);
        virtual_alloc_end += PAGE_SIZE;
        }
    }
    // TODO: case where item is the last item and theres not enough space, in
    // that case we allocate a new page

    // from here, assume that current has enough space.
    uint64_t old_size = current->get_size();
    uint64_t *address = (uint64_t *)current;
    *address = size;
    *(address + 1) = 0xFEEBDAEDDEADBEEF;
    void *result_pointer = address + 2;
    FreeSpace *new_free_list = (FreeSpace *)((uint8_t *)result_pointer + size);
    new_free_list->set_last();
    new_free_list->set_size(old_size - size);
    free_list = new_free_list;
    log(LogLevel::VIRTUAL_MEMORY, "Allocated %i bytes at 0x%x", size,
        result_pointer);
    return result_pointer;
}
