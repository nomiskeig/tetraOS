#include "../include/libk/kstdio.h"
#include "../include/memory.h"
extern uint64_t KERNEL_START;
extern uint64_t KERNEL_END;
extern "C" void switch_to_new_page_table(long);

void paging_init() {
    printf("Initialising paging\n");
    PhysicalFrame first_frame = PhysicalFrame{kalloc_frame()};
    PageTable *first_table = (PageTable *)((char*)first_frame.get_address() + VIRTUAL_OFFSET);
    PageTableEntry* entry = (PageTableEntry*)first_table + 511;
    entry->set_ppn(first_frame.get_ppn());
    printf("entry: 0x%x\n", *entry);
    //first_table->setEntry(511, entry);
    // TODO: map kernel and uart
    // map kernel
    printf("Kernel start: 0x%x\n", KERNEL_START);
    printf("Kernel end:   0x%x\n", KERNEL_END);
    printf("New ppn: 0x%x", first_frame.get_ppn());
    printf("Wowie");
    for (VirtualPage* kernel_page = (VirtualPage*)KERNEL_START; (uint64_t)kernel_page <= KERNEL_END; kernel_page += 512) {
    printf("Kernel  page: 0x%x\n", kernel_page);

    }
    
    //switch_to_new_page_table((uint64_t)first_frame.get_ppn());
}

void map_page(VirtualPage *virtual_page, PhysicalFrame *physical_frame) {
    printf("Mapping virtual page 0x%x to physical_frame 0x%x\n",
           virtual_page->get_address(), physical_frame->get_address());
    // check and map the first level
    uint16_t vpn_3 = virtual_page->get_vpn_3();
    printf("vpn_3: 0x%x\n",vpn_3);
    PageTable *first_level_table = (PageTable *)FIRST_PAGE_TABLE_BASE + vpn_3;
    printf("First entry in table: 1x%x", first_level_table->getEntry(511));
}

void PageTable::setEntry(uint16_t index, PageTableEntry* entry) {
    printf("Setting entry at index %d with entry 0x%x\n", index, entry);
    this->entries[index] = *entry;
}
PageTableEntry* PageTable::getEntry(uint16_t index) {
    return &this->entries[index];
}

void PageTableEntry::set_ppn(ppn_t ppn) {
    // clear the bits first
   // this->entry &= ~(((uint64_t)1 << (PPN_BITS + 1)) - 1);
    this->entry |= ppn << 10;
}
