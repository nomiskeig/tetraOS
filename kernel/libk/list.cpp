#include "../include/libk/list.h"
void *FreeSpace::get_start_address() { return this; }

uint64_t FreeSpace::get_size() { return (this->status >> 1); }

void FreeSpace::set_size(uint64_t size) {
    this->status = ((size << 1) | (this->status & 0x1));
}

bool FreeSpace::is_last() { return (this->status & 0x1) == 0x1; }
void FreeSpace::set_not_last() { this->status = (this->status >> 1) << 1; }
void FreeSpace::set_last() { this->status |= 0x1; }
FreeSpace *FreeSpace::get_next() { return this->next; }
