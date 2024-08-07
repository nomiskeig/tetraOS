#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>

#define VIRTUAL_OFFSET 0xFFFF800000000000
#define FIRST_PAGE_TABLE_BASE 0xFFFFFFFFFFFFF000
#define SECOND_PAGE_TABLE_BASE 0xFFFFFFFFFFE00000
#define THIRD_PAGE_TABLE_BASE 0xFFFFFFFFC0000000
#define FOURTH_PAGE_TABLE_BASE 0xFFFFFF8000000000
#define PPN_BITS 44
#define PAGE_SIZE 4096

typedef uint64_t ppn_t;
class VirtualPage {
private:
    void *virtual_address;

public:
    VirtualPage(void *);
    void *get_address();
    uint16_t get_vpn_3();
    uint16_t get_vpn_2();
    uint16_t get_vpn_1();
    uint16_t get_vpn_0();
    uint16_t get_offset();
};

class PhysicalFrame {
private:
    void *physical_address;

public:
    PhysicalFrame(void *);
    void *get_address();
    void *get_virtual_address();
    ppn_t get_ppn();
};

class PageTableEntry {
private:
    uint64_t entry;
public:
    ppn_t get_ppn();
    void set_ppn(ppn_t ppn);

};

class PageTable {
private:
    PageTableEntry entries[512];
public:
    PageTableEntry* getEntry(uint16_t index);
    void setEntry(uint16_t index, PageTableEntry* entry);
    bool isValid();

};


void physical_allocator_init();
void virtual_allocator_init();
void *kalloc_frame();
void kfree_frame(void *);
void paging_init();
void map_page(VirtualPage *virtual_page, PhysicalFrame *physical_frame);

#endif



