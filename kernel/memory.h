#ifndef MEMORY_H
#define MEMORY_H

#include <tlib/stddef.h>
#include <tlib/stdint.h>

#define VIRTUAL_UART 0xFFFF800000000000 + 0x10000000L
#define VIRTUAL_OFFSET 0xFFFF800000000000
#define FIRST_PAGE_TABLE_BASE 0xFFFFFFFFFFFFF000
#define SECOND_PAGE_TABLE_BASE 0xFFFFFFFFFFE00000
#define THIRD_PAGE_TABLE_BASE 0xFFFFFFFFC0000000
#define FOURTH_PAGE_TABLE_BASE 0xFFFFFF8000000000
#define PPN_BITS 44
#define PAGE_SIZE 4096

typedef uint64_t ppn_t;
typedef uint64_t PageTableEntry;
class VirtualPage {
private:

public:
    void *get_address();
    uint16_t get_vpn_3();
    uint16_t get_vpn_2();
    uint16_t get_vpn_1();
    uint16_t get_vpn_0();
    uint16_t get_offset();
};

class PhysicalFrame {
private:

public:
    void *get_address();
    void *get_virtual_address();
    ppn_t get_ppn();
};


class PageTable {
private:
    PageTableEntry entries[512];
public:
    ppn_t get_ppn();
    PageTableEntry getEntry(uint16_t index);
    PhysicalFrame* getPhysicalFrame(uint16_t index);
    void setEntry(uint16_t index, PageTableEntry entry);
    bool isEntryValid(uint16_t index);

};

extern "C" void *memcpy(void *dest, const void *src, size_t len);
extern "C" void *memset(void* s, int c, size_t n);
void physical_allocator_init();
void virtual_allocator_init();
void *kalloc_frame();
void *kalloc(uint64_t size);
void kfree_frame(void *);
void paging_init();
void map_page(VirtualPage *virtual_page, PhysicalFrame *physical_frame);
void* get_physical_address_of_virtual_address(void * virtual_address);
void memory_barrier();

#endif



