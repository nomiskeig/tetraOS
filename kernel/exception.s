.global jump_back_to_userspace


jump_back_to_userspace:
    ## store the address so we do not lose it
    add t0, zero, a0

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

    # return to usermode
    sret
    

