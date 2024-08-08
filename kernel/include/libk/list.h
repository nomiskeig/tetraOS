#ifndef LIST_H
#define LIST_H

#include <cstdint>
#define ALLOC_PER_ITEM 32
#define ALLOC_PER_ITEM_COUNTER_SIZE uint32_t
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

class AllocList {
private:
    AllocItem first;
    AllocItem *getFirst();
public:
    void set_first(AllocItem *item);
};
#endif
