.equ STACK_SIZE, 8192
.option norvc

.section .text
.global _start

_start: 
csrr a0, mhartid
bnez a0, end
la sp, stacks + STACK_SIZE
j kernel_main


end:
wfi
j end


stacks:
.skip STACK_SIZE
