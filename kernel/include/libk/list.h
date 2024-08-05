#include <cstdint>
class AllocItem {
private:
    void *virtual_address;
    uint64_t size;
    AllocItem *next;
};

class AllocList {
private:
    AllocItem first;
    AllocItem *getFirst();
};
