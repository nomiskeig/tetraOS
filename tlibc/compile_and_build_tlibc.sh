#!/bin/bash
set -e

## crt0.s file is assembled in qemu run script
cd $(dirname $0)

echo "Compiling libc"
rm -rf build/*
riscv64-tetraos-gcc -c -Wl,-melf_riscv64_tetraos syscalls/write.c -o build/write.o
riscv64-tetraos-gcc -c -Wl,-melf_riscv64_tetraos syscalls/read.c -o build/read.o
riscv64-tetraos-gcc -c -Wl,-melf_riscv64_tetraos syscalls/sbrk.c -o build/sbrk.o
riscv64-tetraos-gcc -c -Wl,-melf_riscv64_tetraos syscalls/getcwd.c -o build/getcwd.o
riscv64-tetraos-gcc -c -Wl,-melf_riscv64_tetraos syscalls/syscall.c -o build/syscalls.o
riscv64-tetraos-gcc -c -Wl,-melf_riscv64_tetraos printf.c -o build/printf.o
riscv64-tetraos-gcc -c -Wl,-melf_riscv64_tetraos malloc.c -o build/malloc.o
riscv64-tetraos-gcc -c -Wl,-melf_riscv64_tetraos string.c -o build/string.o
riscv64-tetraos-gcc -c -Wl,-melf_riscv64_tetraos init.c -o build/init.o
rm /home/simon/dev/tetraOS/rootFS/usr/lib/libc.a
riscv64-tetraos-ar cvq /home/simon/dev/tetraOS/rootFS/usr/lib/libc.a build/*.o 
echo "Compiling libc done"
