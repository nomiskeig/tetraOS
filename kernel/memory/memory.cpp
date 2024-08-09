#include "../include/memory.h"


void *PhysicalFrame::get_virtual_address() {
    return (void *)((char *)this->physical_address + VIRTUAL_OFFSET);
}

void *PhysicalFrame::get_address() {
    return this->physical_address;
}

ppn_t PhysicalFrame::get_ppn() {
    return (uint64_t)this->physical_address >> 12;

}

void *VirtualPage::get_address() {
    return this->virtual_address;
}

VirtualPage::VirtualPage(void * address) {
    this->virtual_address = address;

}

uint16_t VirtualPage::get_vpn_3() {
    return (((uint64_t)this->virtual_address) >> 39) & 511;

}
uint16_t VirtualPage::get_vpn_2() {
    return (((uint64_t)this->virtual_address) >> 30) & 511;

}
uint16_t VirtualPage::get_vpn_1() {
    return (((uint64_t)this->virtual_address) >> 21) & 511;

}

uint16_t VirtualPage::get_vpn_0() {
    return (((uint64_t)this->virtual_address) >> 12) & 511;

}
uint16_t VirtualPage::get_offset() {
    return (uint64_t)this->virtual_address & 511;

}

PhysicalFrame::PhysicalFrame(void * address) {
    this->physical_address = address;
}
