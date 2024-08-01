.equ STACK_SIZE, 8192 

.global _start
.global os_start

_start:

   csrr a0, mhartid 
    bnez a0, park
    la sp, stacks + STACK_SIZE
    j os_start

park:
    wfi
    j park

stacks:
    .skip STACK_SIZE * 4
