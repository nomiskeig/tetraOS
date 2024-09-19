#include <tlib/stdint.h>


enum Register {

    a1,
    a2,
    a3,
    a4,
    a5,
    a6,
    a7

};
typedef int64_t register64_t;

register64_t read_register(Register reg);