#include "../include/memory.h"


void *PhysicalFrame::get_virtual_address() {
    return (void *)((char *)this->physical_address + VIRTUAL_OFFSET);
}

void *PhysicalFrame::get_address() {
    return this->physical_address;
}

void *VirtualPage::get_address() {
    return this->virtual_address;
}

VirtualPage::VirtualPage(void * address) {
    this->virtual_address = address;

}
PhysicalFrame::PhysicalFrame(void * address) {
    this->physical_address = address;
}
