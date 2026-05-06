CC = gcc
LD = ld
AS = nasm

# -Iinclude tells GCC to look in the include directory for headers
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -Iinclude
LDFLAGS = -m elf_i386 -T boot/linker.ld

OBJ = boot/boot.o \
      kernel/kmain.o \
      kernel/arch/gdt.o \
      kernel/arch/idt.o \
      kernel/drivers/keyboard.o \
	  kernel/drivers/screen.o \
      kernel/drivers/timer.o \
      kernel/drivers/mouse.o \
      kernel/fs/fs.o

all: night_kernel.bin

night_kernel.bin: $(OBJ)
	$(LD) $(LDFLAGS) -o night_kernel.bin $(OBJ)

# This rule handles all .c files in subfolders
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

boot/boot.o: boot/boot.s
	$(AS) -f elf32 boot/boot.s -o boot/boot.o

clean:
	rm -f $(OBJ) night_kernel.bin