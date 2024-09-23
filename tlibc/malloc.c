#include "tlibc.h"
#include <stdint.h>
#include <string.h>
#include <unistd.h>

// it is important taht allocationitme and freeitem have the same size they can be swapped on (de)allocating
typedef struct AllocationItem {
    uint64_t header;
    size_t size;
    uint64_t padding[2];
} AllocationItem;

typedef struct FreeItem {
    size_t size;
    uint16_t is_last;
    uint16_t is_first;

    struct FreeItem *next_item;
    struct FreeItem *last_item;

} FreeItem;

#define MAGIC_STRING 0xDEADBEEF
static uint64_t HEAP_START;

static uint64_t CURRENT_HEAP_END;
static FreeItem *first_item;
void malloc_init() {
    // this is the first frame on which we store our managing list
    // the list keeps track of the holes in the virtual address space
    // PhysicalFrame physical_address = (PhysicalFrame)(uint64_t*)
    // kalloc_frame(); VirtualPage virtual_page = alloc_list = (AllocList
    // *)physical_address;
    HEAP_START = (uint64_t)sbrk(0);
    first_item = (FreeItem *)HEAP_START;
    first_item->size = PAGE_SIZE;
    first_item->next_item = 0x0;
    first_item->next_item = 0x0;
    first_item->is_last = 1;
    first_item->is_first = 1;
}

// TODO: implement free
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
    FreeItem *current = first_item;
    while (current->size < size && !current->is_last) {
        current = current->next_item;
    }

    if (current->is_last) {
        // TODO: allocate more memory if the last item does not have enough
        // space
        // that requires the brk system call
    }
    // assume that the current item has enough space
    AllocationItem *newItem = (AllocationItem*)current;
    newItem->header = MAGIC_STRING;
    newItem->size = size;
    // append a new free item to the end of the list
    FreeItem * newFreeItem = (FreeItem*) ((uint64_t)newItem + sizeof(AllocationItem) + size);
    newFreeItem->size = current->size - (size + sizeof(AllocationItem));
    first_item = newFreeItem;
    return (void*)((uint64_t) newItem + sizeof(AllocationItem));

}
