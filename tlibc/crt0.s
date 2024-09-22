.section .text


.global _start
_start: 
    # initialize the global pointer
    .option push
    .option norelax
1:  auipc gp, %pcrel_hi(__global_pointer$)
    addi  gp, gp, %pcrel_lo(1b)
    .option pop
    jal initialize_standard_library
    call main
