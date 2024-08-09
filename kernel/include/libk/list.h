#ifndef LIST_H
#define LIST_H

#include <cstdint>
class AllocItem {
private:
    void *virtual_address;
    uint64_t size;
    AllocItem *next;

public:
    void *get_start_address();
    uint64_t get_size();
    void set_size(uint64_t size);
    void set_start_address(void *address);
};

#endif
