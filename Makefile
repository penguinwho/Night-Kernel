CC = i686-elf-gcc
AS = nasm
CFLAGS = -ffreestanding -O2 -Wall -Wextra

# List all objects based on your folders
OBJ = boot/boot.o \
      kernel/kmain.o \
      kernel/arch/gdt.o \
      kernel/arch/idt.o \
      kernel/drivers/keyboard.o \
      kernel/fs/fs.o \
      timer.o

night_kernel.bin: $(OBJ)
	$(CC) -T boot/linker.ld -o night_kernel.bin -nostdlib $(OBJ) -lgcc

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

boot/boot.o: boot/boot.s
	$(AS) -felf32 boot/boot.s -o boot/boot.o