#include "../include/libk/list.h"
 void *AllocItem::get_start_address() { return this->virtual_address; }

void AllocItem::set_start_address(void* address) {
    }

uint64_t AllocItem::get_size() { return this->size; }

void AllocItem::set_size(uint64_t size) { this->size = size; }
