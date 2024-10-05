.global jump_back_to_userspace


jump_back_to_userspace:
    ## store the return value so we do not lose it
    add a3, zero, a1
    ## store the address so we do not lose it
    add t0, zero, a0

    # load s0-s11 from the stack 
    # thats 12 * 8 = 96 bytes
    # see https://stackoverflow.com/questions/59302235/stack-memory-operations-at-the-beginning-of-main-function-in-assembly and https://riscv.org/wp-content/uploads/2015/01/riscv-calling.pdf

    ld s0, 0(sp)
    ld s1, 8(sp)
    ld s2, 16(sp)
    ld s3, 24(sp)
    ld s4, 32(sp)
    ld s5, 40(sp)
    ld s6, 48(sp)
    ld s7, 56(sp)
    ld s8, 64(sp)
    ld s9, 72(sp)
    ld s10, 80(sp)
    ld s11, 88(sp)
    addi sp, sp, 96

    # set the spp to zero so we actually return to userspace regardless of where we came from
    addi a0, zero, 1
    slli a0, a0, 8 
    not a0, a0
    csrr a1, sstatus
    and a0, a1, a0
    csrw sstatus, a0

    # swap the stack pointers of userspace and kernel
    # the pointer to restore is stored in sscratch
    add t1, zero, sp
    csrr sp, sscratch
    csrw sscratch, t1
    
    # jump to the instruction after ecall
    addi t0, t0, 4
    csrw sepc,t0 
    # put the return value in a0
    add a0, zero, a3


    # return to usermode
    sret
    

