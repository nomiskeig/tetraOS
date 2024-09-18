#!/bin/bash
set -e

## crt0.s file is assembled in qemu run script
cd $(dirname $0)

echo "Compiling libc"
rm -rf build/*
riscv64-tetraos-gcc -c -Wl,-melf_riscv64_tetraos printf.c -o build/printf.o
riscv64-tetraos-gcc -c -Wl,-melf_riscv64_tetraos syscalls/write.c -o build/write.o
riscv64-tetraos-ar cvq /home/simon/dev/tetraOS/rootFS/usr/lib/libc.a build/*.o 
echo "Compiling libc done"
