#include "../include/memory.h"


void *PhysicalFrame::get_virtual_address() {
    return (void *)((char *)this + VIRTUAL_OFFSET);
}

void *PhysicalFrame::get_address() {
    return (void*)this;
}

ppn_t PhysicalFrame::get_ppn() {
    return (uint64_t)this >> 12;

}

void *VirtualPage::get_address() {
    return this;
}



uint16_t VirtualPage::get_vpn_3() {
    return (((uint64_t)this) >> 39) & 511;

}
uint16_t VirtualPage::get_vpn_2() {
    return (((uint64_t)this) >> 30) & 511;

}
uint16_t VirtualPage::get_vpn_1() {
    return (((uint64_t)this) >> 21) & 511;

}


uint16_t VirtualPage::get_vpn_0() {
    return (((uint64_t)this) >> 12) & 511;

}
uint16_t VirtualPage::get_offset() {
    return (uint64_t)this & 511;

}

