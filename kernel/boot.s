.equ STACK_SIZE, 8192 

.global _start
.global os_start
.global machine_exception_handler

.section .bss
.boot_page_directory:
    .skip 4096
.boot_first_level_table:
    .skip 4096
.boot_second_level_table:
    .skip 4096
.boot_third_level_table: 
    .skip 4096

.boot_uart_first_level_table:
    .skip 4096
.boot_uart_second_level_table:
    .skip 4096
.boot_uart_third_level_table:
    .skip 4096

.section .machine

.section .text
_start:
   csrr a0, mhartid 
    bnez a0, park
   la sp, stacks + STACK_SIZE


    # map the addresses
    # s9 holds the virtual end of the kernel
    lui s10, %hi(0x80000000)
    slli s10, s10, 16
    la s9, _kernel_end
    # map 280 extra pages to give some space for initialization in cpp, this is required for the initialization of the phyisical memory
    addi a2, zero, 280 
    slli a2, a2, 12
    add s9, a2, s9



    add s9, s10, s9
    # a0 holds the virtual start of the kernel

    lui a0, %hi(0x80000000)
    SLLI a0, a0, 16
    la a2, _kernel_start
    add a0, a0, a2
    # s7 holds the pyhsical address, s8 the virtual
    add s7, a2, zero
    add s8, a0, zero
    jal boot_map_address
    j map_next_address

map_next_address: 
    addi s11, zero, 0x1
    slli s11, s11, 12
    add s7, s7, s11
    add s8, s8, s11

    bge s8,s9, jump_to_c
    # write s8 to a0 since the method expects it
    add a0, s8, zero
    # TODO: check bounds and jump to jump_to_c of bounds are correct
    jal boot_map_address
    j map_next_address;
    

jump_to_machine_exception_handler:
    # jump to c machine exception handler

    # store mcause in t0 in order to access it in user mode 
    # TODO: this should be handled better, it will break once we dont want to just die on exceptino
    csrr t0, mcause

    # store mtval in t1, this is the address at whihc it faield

    # swap the stack pointers of userspace and kernel
    # the pointer to restore is stored in sscratch
    # sp is now the kernel sp
    add t1, zero, sp
    csrr sp, sscratch
    csrw sscratch, t1


    # put s0-s11 on the stack 
    # thats 12 * 8 = 96 bytes
    # see https://stackoverflow.com/questions/59302235/stack-memory-operations-at-the-beginning-of-main-function-in-assembly and https://riscv.org/wp-content/uploads/2015/01/riscv-calling.pdf

    addi sp, sp, -96
    sd s0, 0(sp)
    sd s1, 8(sp)
    sd s2, 16(sp)
    sd s3, 24(sp)
    sd s4, 32(sp)
    sd s5, 40(sp)
    sd s6, 48(sp)
    sd s7, 56(sp)
    sd s8, 64(sp)
    sd s9, 72(sp)
    sd s10, 80(sp)
    sd s11, 88(sp)



    csrr t1, mepc
    csrr t2, mepc
    addi t3, zero, 0
    lui t3, %hi(0x80000000)
    slli t3, t3, 16
    la t4, machine_exception_handler
    add t3, t3, t4 
    csrw mepc, t3 
    # set the MPP bit in mstatus to 1 so we jump to supervisor mode and not to usermode
    addi t3, zero, 1
    slli t3, t3, 11
    csrr t4, mstatus
    or t3, t4,t3 
    csrw mstatus, t3
    mret 
    
jump_to_supervisor_exception_handler:
    # jump to c machine exception handler

    # store mcause in t0 in order to access it in user mode 
    # TODO: this should be handled better, it will break once we dont want to just die on exceptino
    csrr t0, scause
    addi a2, zero, 0
    lui a2, %hi(0x80000000)
    slli a2, a2, 16
    la a3, supervisor_exception_handler
    j supervisor_exception_handler
    add a2, a2, a3
#    csrw sepc, a2
#    sret 


jump_to_c:
    # enable interrupts
    # map the addresses
    # s9 holds the virtual end of the kernel
    #addi a2, zero, 0
    #lui a2, %hi(0x80000000)
    #slli a2, a2, 16
    la a3, jump_to_machine_exception_handler
    #add a2, a2, a3
    csrw mtvec, a3
    
    # add exception handler for user mode
    addi a2, zero, 0
    lui a2, %hi(0x80000000)
    slli a2, a2, 16
    la a3, jump_to_supervisor_exception_handler
    add a2, a2, a3
    csrw stvec, a2
    
    # map the UART thingy
    jal boot_uart_map_address


    # write the satp register 
    la a5, .boot_page_directory
    addi a6, zero, 9 # 9 for 48 bit virtual addressing, see riscv spec
    slli a6, a6, 20
    slli a6, a6, 20
    slli a6, a6, 20
    srli a5, a5, 12
    or a5, a5, a6
    csrw satp, a5


    # prepare to jump to os at virtual address
    addi t0, zero, 0
    lui t0, %hi(0x80000000)
    SLLI t1, t0, 16
    la a2, os_start
    add t2, t1, a2
    # prepare mret instruction
    # set mepc to os_strt
    csrw mepc, t2
    # set mstatus.mpp to s
    # so we set bit 12 to zero
    # since the last mode is not available, we read 00 for bits 12 and 11 so we just set bit 11
    addi a1, zero, 1
    slli a1, a1, 11
    csrr a2, mstatus
    or a1, a1, a2
    csrw mstatus, a1


    # see https://www.reddit.com/r/RISCV/comments/uhcrf7/comment/i75amb4/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
    # handle pmp
    addi a1, zero, 0x1f
    csrw pmpcfg0, a1

    add a1, zero, zero
    not a1, a1
    csrw pmpaddr0, a1

    ## adjust stack pointer to use virtual adresses as well
    addi a3, zero, 0
    lui a3, %hi(0x80000000)
    SLLI a3, a3, 16
    add sp, sp, a3


    mret



park:
    wfi
    j park

stacks:
    .skip STACK_SIZE * 4


    # assumes that the page is in a0
boot_map_address:
    # first, get the highest 9 bits used and store the result into t1
    srli a1, a0, 20 # shift right 39 bits
    srli a1, a1, 19 # shift right 39 bits
    andi a1, a1, 511  # and with 9 bits
    # vpn 2 in a2
    srli a2, a0, 20
    srli a2, a2, 10
    andi a2, a2, 511
    # vpn 1 in a3
    srli a3, a0, 21
    andi a3, a3, 511
    # vpn 0 in a4
    srli a4, a0, 12 
    andi a4, a4, 511

    # set .boot_page_directory[a1] = .boot_first_level_table
    la a5, .boot_page_directory
    addi s1, zero, 8
    mul s1, s1, a1
    add s1, s1, a5
    la s2, .boot_first_level_table
    #slli s2, s2, 10
    srli s2,s2,2
    addi s3, zero, 0x1
    or s2, s2, s3 # make the entry valid
    sd s2, 0(s1)
    
    # set .boot_first_level_table[a2] = .boot_second_level_table
    la a5, .boot_first_level_table
    addi s1, zero, 8
    mul s1, s1, a2
    add s1, s1, a5
    la s2, .boot_second_level_table
    #slli s2, s2, 10
    srli s2,s2,2
    addi s3, zero, 0x1
    or s2, s2, s3 # make the entry valid
    sd s2, 0(s1)


    # set .boot_second_level_table[a3] = .boot_third_level_table
    la a5, .boot_second_level_table
    addi s1, zero, 8
    mul s1, s1, a3
    add s1, s1, a5
    la s2, .boot_third_level_table
    #slli s2, s2, 10
    srli s2,s2,2
    addi s3, zero, 0x1
    or s2, s2, s3 # make the entry valid
    sd s2, 0(s1)

    # set .boot_third_lebel_table[a4] = physical address
    la a5, .boot_third_level_table
    addi s1, zero, 8
    mul s1, s1, a4
    add s1, s1, a5
    # s7 holds the pyhsical address
    add s2, zero, s7
    #slli s2, s2, 10
    srli s2,s2,2
    addi s3, zero, 0xF
    or s2, s2, s3 # make the entry valid, and make pte readable, writable and executable
    sd s2, 0(s1)

    ret



    


    
boot_uart_map_address:
    # store the virtual adress in a0 
    lui a0, %hi(0x80000000)
    SLLI a0, a0, 16
    la a1, _uart 
    add a0, a1, a0 

    #store the pyhsical address in s7
    la s7, _uart

    # first, get the highest 9 bits used and store the result into t1
    srli a1, a0, 20 # shift right 39 bits
    srli a1, a1, 19 # shift right 39 bits
    andi a1, a1, 511  # and with 9 bits
    # vpn 2 in a2
    srli a2, a0, 20
    srli a2, a2, 10
    andi a2, a2, 511
    # vpn 1 in a3
    srli a3, a0, 21
    andi a3, a3, 511
    # vpn 0 in a4
    srli a4, a0, 12 
    andi a4, a4, 511

    
    # set .boot_first_level_table[a2] = .boot_second_level_table
    la a5, .boot_first_level_table
    addi s1, zero, 8
    mul s1, s1, a2
    add s1, s1, a5
    la s2, .boot_uart_second_level_table
    #slli s2, s2, 10
    srli s2,s2,2
    addi s3, zero, 0x1
    or s2, s2, s3 # make the entry valid
    sd s2, 0(s1)


    # set .boot_second_level_table[a3] = .boot_third_level_table
    la a5, .boot_uart_second_level_table
    addi s1, zero, 8
    mul s1, s1, a3
    add s1, s1, a5
    la s2, .boot_uart_third_level_table
    #slli s2, s2, 10
    srli s2,s2,2
    addi s3, zero, 0x1
    or s2, s2, s3 # make the entry valid
    sd s2, 0(s1)

    # set .boot_third_lebel_table[a4] = physical address
    la a5, .boot_uart_third_level_table
    addi s1, zero, 8
    mul s1, s1, a4
    add s1, s1, a5
    # s7 holds the pyhsical address
    add s2, zero, s7
    #slli s2, s2, 10
    srli s2,s2,2
    addi s3, zero, 0xF
    or s2, s2, s3 # make the entry valid, and make pte readable, writable and executable
    sd s2, 0(s1)

    ret



    
