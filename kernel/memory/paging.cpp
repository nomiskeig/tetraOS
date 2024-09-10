#include <kernel/libk/kstdio.h>
#include <kernel/memory.h>
extern uint64_t KERNEL_START;
extern uint64_t KERNEL_END;
extern uint64_t PHYSICAL_MEMORY_END;
static PageTable *first_level_table;
extern "C" void switch_to_new_page_table(long);
#define GIGAYBTE_BYTES 0x40000000
void map_page_for_setup(PageTable *first_table, PageTable *second_table,
                        VirtualPage *page_to_map, PhysicalFrame *frame);

PageTableEntry createValidPageTableEntry(ppn_t ppn) {
    return ((ppn & 0xFFFFFFFFFF) << 10) | 1;
}
bool isValidPageTableEntry(PageTableEntry entry) { return entry & 1; };
void paging_init() {
    log(LogLevel::PAGING, "Initialising paging");
    PhysicalFrame *first_frame = (PhysicalFrame *)kalloc_frame();
    PhysicalFrame *second_frame = (PhysicalFrame *)kalloc_frame();
    PageTable *first_table =
        (PageTable *)((char *)first_frame->get_address() + VIRTUAL_OFFSET);

    first_level_table = first_table;
    PageTable *second_table =
        (PageTable *)((char *)second_frame->get_address() + VIRTUAL_OFFSET);
    log(LogLevel::PAGING, "First level table: 0x%x", first_table);
    log(LogLevel::PAGING, "Second level table: 0x%x", second_table);
    for (uint64_t kernel_page = VIRTUAL_OFFSET;
         (uint64_t)kernel_page < VIRTUAL_OFFSET + (uint64_t)4 * GIGAYBTE_BYTES;
         kernel_page += GIGAYBTE_BYTES) {
        map_page_for_setup(first_table, second_table,
                           (VirtualPage *)kernel_page,
                           (PhysicalFrame *)(kernel_page - VIRTUAL_OFFSET));
    }

    // map the first table
    switch_to_new_page_table((uint64_t)first_frame->get_ppn());
    log(LogLevel::PAGING, "Paging initializion done");
}

// DO NOT USE IF YOU DO NOT KNOW WHAT YOU ARE DOING
// This method can not map arbitrary pages, use the map_page function instead
// This method maps pages assuming that they are all mapped by the same four
// tables
//
void map_page_for_setup(PageTable *first_table, PageTable *second_table,
                        VirtualPage *page_to_map, PhysicalFrame *frame) {
    log(LogLevel::PAGING,
        "Mapping page for initializion: page: 0x%x, frame: 0x%x", page_to_map,
        frame);
    uint16_t vpn_3 = page_to_map->get_vpn_3();
    uint16_t vpn_2 = page_to_map->get_vpn_2();
    uint16_t vpn_1 = page_to_map->get_vpn_1();
    uint16_t vpn_0 = page_to_map->get_vpn_0();
    first_table->setEntry(vpn_3,
                          createValidPageTableEntry(second_table->get_ppn()));
    second_table->setEntry(vpn_2,
                           createValidPageTableEntry(frame->get_ppn()) | 0xF);
}

void map_page(VirtualPage *virtual_page, PhysicalFrame *physical_frame,
              uint16_t flag) {
    log(LogLevel::PAGING,
        "Mapping virtual page 0x%x to physical frame 0x%x with flags 0x%x",
        virtual_page->get_address(), physical_frame->get_address(), flag);

    // check and map the first level
    uint16_t vpn_3 = virtual_page->get_vpn_3();

    // check for invalid entry in first table
    if (!first_level_table->isEntryValid(virtual_page->get_vpn_3())) {
        // allocate new table and point to it
        PhysicalFrame *frame = (PhysicalFrame *)kalloc_frame();
        first_level_table->setEntry(
            virtual_page->get_vpn_3(),
            createValidPageTableEntry(frame->get_ppn()));
    }
    PageTable *second_table = (PageTable *)(first_level_table->getPhysicalFrame(
                                                virtual_page->get_vpn_3()) +
                                            VIRTUAL_OFFSET);
    // check for invalid entry in second table
    if (!second_table->isEntryValid(virtual_page->get_vpn_2())) {
        // allocate new table and point to it
        PhysicalFrame *frame = (PhysicalFrame *)kalloc_frame();
        second_table->setEntry(virtual_page->get_vpn_2(),
                               createValidPageTableEntry(frame->get_ppn()));
    }
    PageTable *third_table = (PageTable *)(second_table->getPhysicalFrame(
                                               virtual_page->get_vpn_2()) +
                                           VIRTUAL_OFFSET);
    // check for invalid entry in third table
    if (!third_table->isEntryValid(virtual_page->get_vpn_1())) {
        // allocate new table and point to it
        PhysicalFrame *frame = (PhysicalFrame *)kalloc_frame();
        third_table->setEntry(virtual_page->get_vpn_1(),
                              createValidPageTableEntry(frame->get_ppn()));
    }
    PageTable *fourth_table =
        (PageTable *)(third_table->getPhysicalFrame(virtual_page->get_vpn_1()) +
                      VIRTUAL_OFFSET);
    fourth_table->setEntry(
        virtual_page->get_vpn_0(),
        createValidPageTableEntry(physical_frame->get_ppn()) | flag);
}
// Returns the physical address the provided virtual address is mapped to.
// Retuns a null pointer if the virtual address is not mapped
void *get_physical_address_of_virtual_address(void *virtual_address) {
    // we assume that the page is already mapped
    VirtualPage *virtual_page = (VirtualPage *)virtual_address;
    // check and map the first level
    uint16_t vpn_3 = virtual_page->get_vpn_3();

    // check for invalid entry in first table
    if (!first_level_table->isEntryValid(vpn_3)) {
        log(LogLevel::ERROR,
            "Translating virtual address went wrong: Index in first level "
            "table at 0x%x is invalid.",
            first_level_table);
        return 0;
    }
    PageTable *second_table = (PageTable *)(first_level_table->getPhysicalFrame(
                                                virtual_page->get_vpn_3()) +
                                            VIRTUAL_OFFSET);
    // check for invalid entry in second table
    if (!second_table->isEntryValid(virtual_page->get_vpn_2())) {
        log(LogLevel::ERROR,
            "Translating virtual address went wrong: Index in second level "
            "table at 0x%x is invalid.",
            second_table);
        return 0;
    }
    PageTable *third_table = (PageTable *)(second_table->getPhysicalFrame(
                                               virtual_page->get_vpn_2()) +
                                           VIRTUAL_OFFSET);
    // check for invalid entry in third table
    if (!third_table->isEntryValid(virtual_page->get_vpn_1())) {
        log(LogLevel::ERROR,
            "Translating virtual address went wrong: Index in third level "
            "table at 0x%x is invalid.",
            third_table);
        return 0;
    }
    PageTable *fourth_table =
        (PageTable *)(third_table->getPhysicalFrame(virtual_page->get_vpn_1()) +
                      VIRTUAL_OFFSET);
    // check for invalid entry in the fourth table
    if (!fourth_table->isEntryValid(virtual_page->get_vpn_0())) {
        log(LogLevel::ERROR,
            "Translating virtual address went wrong: Index in fourth level "
            "table at 0x%x is invalid.",
            fourth_table);
        return 0;
    }
    uint64_t offset = virtual_page->get_offset();
    return ((uint64_t *)((uint64_t)fourth_table->getPhysicalFrame(
                             virtual_page->get_vpn_0()) |
                         offset));
}

void PageTable::setEntry(uint16_t index, PageTableEntry entry) {
    this->entries[index] = entry;
}
PageTableEntry PageTable::getEntry(uint16_t index) {
    return this->entries[index];
}
PhysicalFrame *PageTable::getPhysicalFrame(uint16_t index) {
    return (PhysicalFrame *)((this->entries[index] >> 10) << 12);
}
uint64_t PageTable::get_ppn() {
    return (((uint64_t)this - VIRTUAL_OFFSET) >> 12) & 0xFFFFFFFFFFF;
}

bool PageTable::isEntryValid(uint16_t index) {
    return (this->entries[index] & 0x1) == 0x1;
}
