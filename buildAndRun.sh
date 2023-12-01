#!/bin/sh


build() {
    cd build_out
    make clean
    cmake -DCMAKE_LINKER=riscv64-unknown-elf-gcc -DCMAKE_CXX_COMPILER=riscv64-unknown-elf-g++ -DCMAKE_C_COMPILER=riscv64-unknown-elf-gcc ..
    make 
}


run() {
    qemu-system-riscv64 -nographic -machine virt -m 128M -smp 4 -drive if=none,format=raw,file=../hdd.dsk  -serial mon:stdio -bios none -device virtio-rng-device -device virtio-gpu-device -device virtio-net-device -device virtio-tablet-device -device virtio-keyboard-device -kernel Kernel/Kernel
}
build
run
