#ifndef LIST_H
#define LIST_H

#include <tlib/stdint.h>
class FreeSpace {
private:
    uint64_t status;
    FreeSpace *next;


public:
    bool is_last();
    void set_last();
    void set_not_last();
    FreeSpace* get_next();
    void *get_start_address();
    uint64_t get_size();
    void set_size(uint64_t size);
    void set_start_address(void *address);
};

#endif
