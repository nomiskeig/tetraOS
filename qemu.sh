#!/bin/sh
set -e
debug='false'

print_usage() {
    printf "Usage:\n"
    printf "'-d' Flag activates debug mode"
}
# see https://stackoverflow.com/questions/7069682/how-to-get-arguments-with-flags-in-bash
while getopts 'd' flag; do
    case "${flag}" in
        d) debug='true';;
        *) print_usage
            exit 1 ;;
    esac
done

echo ${debug}

# build the kernel
make kernel

./clean_root_fs.sh

## tlbic
cp -r tlibc/include/* rootFS/usr/include || true


## assemble crt0.s
riscv64-tetraos-as tlibc/crt0.s -o rootFS/usr/lib/crt0.o

tlibc/compile_and_build_tlibc.sh
userspace/compile_and_build_userspace.sh

mkdir datafs
# create loopback device for the file
sudo losetup -P /dev/loop0 data
# mount the partition 
sudo mount /dev/loop0p1 datafs


## copy the contents from rootFS to the mounted device
cp -r rootFS/* datafs 


# create the binary file
riscv64-unknown-elf-objcopy -O binary build/kernel build/kernel.bin

# copy the kernel file
cp build/kernel.bin datafs/

# create the uboot script
# see https://krinkinmu.github.io/2023/11/19/u-boot-boot-script.html
mkimage -T script -d uboot.txt build/boot.scr

# copy the script
#
cp build/boot.scr datafs/boot



# unmount and remove loop device
sudo umount datafs
sudo losetup -d /dev/loop0
# run qemu

rm -rf datafs

if [[ "$debug" == "true" ]]; then
    qemu-system-riscv64 -machine virt -nographic  -bios u-boot.bin -drive if=none,format=raw,file=./data,id=foo -device virtio-blk-device,scsi=off,drive=foo -device virtio-net-device -m 2048M -global virtio-mmio.force-legacy=false -s -S
else 
    qemu-system-riscv64 -machine virt -nographic  -bios u-boot.bin -drive if=none,format=raw,file=./data,id=foo -device virtio-blk-device,scsi=off,drive=foo -device virtio-net-device -m 2048M -global virtio-mmio.force-legacy=false
fi



