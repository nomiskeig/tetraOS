
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <kernel/processor.h>

/*
 * See https://man7.org/linux/man-pages/man2/syscall.2.html
    */
struct SyscallParameters {
    register64_t a0;
    register64_t a1;
    register64_t a2;
    register64_t a3;
    register64_t a4;
    register64_t a5;
    register64_t a7;

};

#endif

