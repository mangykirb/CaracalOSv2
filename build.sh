#!/bin/bash
set -e

echo "[Floppa says: FINALLY! YOUVE ACTUALLY RAN THIS BUILD.SH! CONGRATS FOR TYPING 10 CHARACTERS AND PRESSING ENTER!"

rm -f boot.bin kernel.bin kernel.elf kernel.map \
kernel.o kernel_entry.o display.o gdt.o gdt_c.o \
idt.o isr.o interrupts.o user.o user_c.o paging.o test_exception.o os.bin

CFLAGS="-m32 -ffreestanding -fno-pie -fno-pic \
-fno-stack-protector -fno-builtin -nostdlib \
-Ikernel/include"

nasm -f bin bootloader/boot.asm -o boot.bin


gcc $CFLAGS -c kernel/kernel.c -o kernel.o
gcc $CFLAGS -c kernel/cpu/gdt.c -o gdt_c.o
gcc $CFLAGS -c kernel/cpu/idt.c -o idt.o
gcc $CFLAGS -c kernel/cpu/paging.c -o paging.o
gcc $CFLAGS -c kernel/cpu/isr.c -o isr.o
gcc $CFLAGS -O0 -c user/hello.c -o user_c.o
gcc $CFLAGS -c kernel/cpu/test_exception.c -o test_exception.o

nasm -f elf32 bootloader/kernel_entry.asm -o kernel_entry.o
nasm -f elf32 kernel/asm/display.asm -o display.o
nasm -f elf32 kernel/asm/gdt.asm -o gdt.o
nasm -f elf32 kernel/asm/interrupts.asm -o interrupts.o
nasm -f elf32 kernel/asm/user.asm -o user.o


ld -m elf_i386 -T linker.ld -Map=kernel.map \
-o kernel.elf \
kernel_entry.o kernel.o display.o \
gdt.o gdt_c.o idt.o isr.o interrupts.o user.o user_c.o paging.o \
test_exception.o


objcopy -O binary kernel.elf kernel.bin

SIZE=$(stat -c%s kernel.bin)


if [ "$SIZE" -gt 32768 ]; then
    echo "[ERROR] Kernel exceeds 20-sector limit!"
    exit 1
fi

cat boot.bin kernel.bin > os.bin
echo "[CARACAL] Build successful!"
echo "[Damn howdya do that]"
qemu-system-i386 \
-drive format=raw,file=os.bin \
-vga std \
-debugcon stdio \
-global isa-debugcon.iobase=0xe9

