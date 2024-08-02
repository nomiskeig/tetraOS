.equ STACK_SIZE, 8192 

.global _start
.global os_start

_start:
   csrr a0, mhartid 
    bnez a0, park
    la sp, stacks + STACK_SIZE

    # prepare to jump to os at virtual address
    lui t0, %hi(0x80000000)
    SLLI t1, t0, 16
    la a2, os_start
    add t2, t1, a2
    jr t2

park:
    wfi
    j park

stacks:
    .skip STACK_SIZE * 4
