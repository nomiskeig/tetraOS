#!/bin/sh

clean() {
    rm -rf build_out_hand
    mkdir build_out_hand
}
build() {

    # build boot.S
    riscv32-unknown-elf-gcc  -ffreestanding -O0 -Wall -Wextra -fno-exceptions -fno-rtti -c kernel/asm/boot.S -o build_out_hand/boot.o 
    # build main.cpp
    riscv32-unknown-elf-g++  -c kernel/main.cpp -o build_out_hand/kernel.cpp.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -MD -MT build_out_hand/kernel.cpp.o -MF build_out_hand/main.cpp.o.d
    # link
    riscv32-unknown-elf-g++  -T kernel/linker.ld -o build_out_hand/kernel.elf -ffreestanding -O2 -nostdlib -lgcc -fno-builtin build_out_hand/boot.o build_out_hand/kernel.cpp.o 
}
run() {
    qemu-system-riscv32 -nographic -machine virt -m 128M -smp 4 -drive if=none,format=raw,file=./hdd.dsk -bios none -kernel build_out_hand/kernel.elf 
}

clean
build
run
