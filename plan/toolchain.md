The target is called elf-riscv64-tetraos.sh

I dont know why, but this command seems to do the trick:
riscv64-tetraos-gcc -Wl,-melf_riscv_tetraos ...
