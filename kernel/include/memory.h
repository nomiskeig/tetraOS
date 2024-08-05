#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#define VIRTUAL_OFFSET 0xFFFF800000000000
class VirtualPage {
private:
    void *virtual_address;

public:
    VirtualPage(void *);
    void *get_address();
};

class PhysicalFrame {
private:
    void *physical_address;

public:
    PhysicalFrame(void *);
    void *get_address();
    void *get_virtual_address();
};

void physical_allocator_init();
void virtual_allocator_init();
void *kalloc_frame();
void kfree_frame(void *);
void map_page(VirtualPage *virtual_page, PhysicalFrame *physical_frame);

#endif
