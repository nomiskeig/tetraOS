.global switch_to_process
.global set_spp_zero
.global set_sum_bit

switch_to_process:
    # a0 holds the virtual start address
    # a1 holds the stack pointer
    csrw sepc, a0
    # store the old stack pointer in sscratch
    csrw sscratch, sp
    add sp, a1, zero
    sret


set_sum_bit:
    # set bit 18 of sstatus
    addi a0, zero, 1
    slli a0, a0, 18
    csrr a1, sstatus
    or a0, a0, a1
    csrw sstatus, a0


set_spp_zero:
    addi a0, zero, 1
    slli a0, a0, 8 
    not a0, a0
    csrr a1, sstatus
    and a0, a1, a0
    csrw sstatus, a0
