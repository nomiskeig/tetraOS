.global switch_to_new_page_table

switch_to_new_page_table:
    add a5, zero, a0 
    addi a6, zero, 9 # 9 for 48 bit virtual addressing, see riscv spec
    slli a6, a6, 20
    slli a6, a6, 20
    slli a6, a6, 20
    or a5, a5,a6 
    csrw satp, a5
    sfence.vma
    ret

