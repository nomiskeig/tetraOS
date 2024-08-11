#include "../include/libk/kstdio.h"
#include "../include/memory.h"
extern uint64_t KERNEL_START;
extern uint64_t KERNEL_END;
extern "C" void switch_to_new_page_table(long);
void map_page_for_setup(PageTable *first_table, PageTable *second_table,
                        PageTable *third_table, PageTable *fourth_table,
                        VirtualPage *page_to_map, PhysicalFrame *frame);

PageTableEntry createValidPageTableEntry(ppn_t ppn) { return ((ppn & 0xFFFFFFFFFF)<< 10) | 1; }
bool isValidPageTableEntry(PageTableEntry entry) {return entry & 1;};
void paging_init() {
    printf("Initialising paging\n");
    PhysicalFrame* first_frame = (PhysicalFrame*)kalloc_frame();
    PhysicalFrame* second_frame = (PhysicalFrame*)kalloc_frame();
    PhysicalFrame* third_frame = (PhysicalFrame*)kalloc_frame();
    PhysicalFrame* fourth_frame = (PhysicalFrame*)kalloc_frame();
    PhysicalFrame* uart_second_table_frame = (PhysicalFrame*)kalloc_frame();
    PhysicalFrame* uart_third_table_frame = (PhysicalFrame*)kalloc_frame();
    PhysicalFrame* uart_fourth_table_frame = (PhysicalFrame*)kalloc_frame();
    PageTable *first_table =
        (PageTable *)((char *)first_frame->get_address() + VIRTUAL_OFFSET);
    PageTable *second_table =
        (PageTable *)((char *)second_frame->get_address() + VIRTUAL_OFFSET);
    PageTable *third_table =
        (PageTable *)((char *)third_frame->get_address() + VIRTUAL_OFFSET);
    PageTable *fourth_table =
        (PageTable *)((char *)fourth_frame->get_address() + VIRTUAL_OFFSET);
    PageTable *second_uart_table =
        (PageTable *)((char *)uart_second_table_frame->get_address() + VIRTUAL_OFFSET);
    PageTable *third_uart_table =
        (PageTable *)((char *)uart_third_table_frame->get_address() + VIRTUAL_OFFSET);
    PageTable *fourth_uart_table =
        (PageTable *)((char *)uart_fourth_table_frame->get_address() + VIRTUAL_OFFSET);
    // set the recursion
    printf("recuring entry in first table: 0x%x with ppn 0x%x\n", createValidPageTableEntry(first_table->get_ppn() ), first_table->get_ppn());
    first_table->setEntry(511,
                          createValidPageTableEntry(first_table->get_ppn()) | 0xF);
    printf("Kernel start: 0x%x\n", KERNEL_START);
    printf("Kernel end:   0x%x\n", KERNEL_END);
    printf("New ppn: 0x%x\n", first_frame->get_ppn());
    printf("Wowie\n");
    // map the kernel into the new kernel by hand, we can not use the map_page
    // method as it uses the recursive page table that is not set up in the boot
    // table mapping and it would use the old table which does not help us
    // we also map 8 pages for the physical memory management
    for (VirtualPage *kernel_page = (VirtualPage *)KERNEL_START;
         (uint64_t)kernel_page <= KERNEL_END + 4096 * 8; kernel_page += 4096) {
        printf("Kernel  page: 0x%x\n", kernel_page);
        map_page_for_setup(
            first_table, second_table, third_table, fourth_table, kernel_page,
            (PhysicalFrame *)((char *)kernel_page - VIRTUAL_OFFSET));
    }

    // map the uart thingy
    VirtualPage* uart_page = (VirtualPage*)VIRTUAL_UART;
    PhysicalFrame* uart_frame = (PhysicalFrame*)VIRTUAL_UART - VIRTUAL_OFFSET;
    map_page_for_setup(
        first_table, second_table, third_uart_table, fourth_uart_table, uart_page, uart_frame);


    printf("switching to new table with ppn 0x%x\n", first_frame->get_ppn());

    switch_to_new_page_table((uint64_t)first_frame->get_ppn());
}

// DO NOT USE IF YOU DO NOT KNOW WHAT YOU ARE DOING
// This method can not map arbitrary pages, use the map_page function instead
// This method maps pages assuming that they are all mapped by the same four
// tables
void map_page_for_setup(PageTable *first_table, PageTable *second_table,
                        PageTable *third_table, PageTable *fourth_table,
                        VirtualPage *page_to_map, PhysicalFrame *frame) {
    printf("page: 0x%x, frame: 0x%x\n", page_to_map, frame);
    uint16_t vpn_3 = page_to_map->get_vpn_3();
    uint16_t vpn_2 = page_to_map->get_vpn_2();
    uint16_t vpn_1 = page_to_map->get_vpn_1();
    uint16_t vpn_0 = page_to_map->get_vpn_0();
    printf("frame: 0x%x\n", frame);
    printf("ppn of frame: 0x%x\n", frame->get_ppn());
    printf("vpn_3: 0x%x\n", vpn_3);
    printf("vpn_2: 0x%x\n", vpn_2);
    printf("vpn_1: 0x%x\n", vpn_1);
    printf("vpn_0: 0x%x\n", vpn_0);
    first_table->setEntry(vpn_3,
                          createValidPageTableEntry(second_table->get_ppn()));
    second_table->setEntry(vpn_2,
                           createValidPageTableEntry(third_table->get_ppn()));
    third_table->setEntry(vpn_1,
                          createValidPageTableEntry(fourth_table->get_ppn()));
    fourth_table->setEntry(vpn_0, createValidPageTableEntry(frame->get_ppn()) | 0xF);
}

void map_page(VirtualPage *virtual_page, PhysicalFrame *physical_frame) {
    printf("Mapping virtual page 0x%x to physical_frame 0x%x\n",
           virtual_page->get_address(), physical_frame->get_address());

    // check and map the first level
    uint16_t vpn_3 = virtual_page->get_vpn_3();
    printf("vpn_3: 0x%x\n", vpn_3);
    PageTable *first_level_table = (PageTable *)FIRST_PAGE_TABLE_BASE;
    printf("First entry in table: 1x%x", first_level_table->getEntry(511));
}

void PageTable::setEntry(uint16_t index, PageTableEntry entry) {
    printf("Setting entry at index %d with entry 0x%x\n", index, entry);
    this->entries[index] = entry;
}
PageTableEntry PageTable::getEntry(uint16_t index) {
    return this->entries[index];
}
uint64_t PageTable::get_ppn() { return (((uint64_t)this - VIRTUAL_OFFSET) >> 12) & 0xFFFFFFFFFFF; }

