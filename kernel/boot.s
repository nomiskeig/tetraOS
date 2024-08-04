.equ STACK_SIZE, 8192 

.global _start
.global os_start

.section .bss
.boot_page_directory:
    .skip 4096
.boot_first_level_table:
    .skip 4096
.boot_second_level_table:
    .skip 4096
.boot_third_level_table: 
    .skip 4096


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
    add s9, s10, s9
    # a0 holds the virtual start of the kernel

    lui a0, %hi(0x80000000)
    SLLI a0, a0, 16
    la a2, _kernel_start
    add a0, a0, a2
    # s7 holds the pyhsical address, s8 the virtual
    add s7, a2, zero
    add s8, a0, zero
    j boot_map_address

map_next_address: 
    addi s11, zero, 0x1
    slli s11, s11, 12
    add s7, s7, s11
    add s8, s8, s11

    bge s8,s9, jump_to_c
    # write s8 to a0 since the method expects it
    add a0, s8, zero
    # TODO: check bounds and jump to jump_to_c of bounds are correct
    j boot_map_address
    


jump_to_c:

    # write the satp register 
    la a5, .boot_page_directory
    addi a6, zero, 9 # 9 for 48 bit virtual addressing, see riscv spec
    slli a6, a6, 20
    slli a6, a6, 20
    slli a6, a6, 20
    add a5, a5, a6
    csrw satp, a5


    # prepare to jump to os at virtual address
    addi t0, zero, 0
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
    slli s2, s2, 10
    addi s3, zero, 0x1
    or s2, s2, s3 # make the entry valid
    sd s2, 0(s1)
    
    # set .boot_first_level_table[a2] = .boot_second_level_table
    la a5, .boot_first_level_table
    addi s1, zero, 8
    mul s1, s1, a2
    add s1, s1, a5
    la s2, .boot_second_level_table
    slli s2, s2, 10
    addi s3, zero, 0x1
    or s2, s2, s3 # make the entry valid
    sd s2, 0(s1)


    # set .boot_second_level_table[a3] = .boot_third_level_table
    la a5, .boot_second_level_table
    addi s1, zero, 8
    mul s1, s1, a3
    add s1, s1, a5
    la s2, .boot_third_level_table
    slli s2, s2, 10
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
    slli s2, s2, 10
    addi s3, zero, 0xF
    or s2, s2, s3 # make the entry valid, and make pte readable, writable and executable
    sd s2, 0(s1)


    j map_next_address



    

