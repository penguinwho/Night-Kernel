; --- Multiboot Header ---
MBOOT_PAGE_ALIGN    equ 1<<0
MBOOT_MEM_INFO      equ 1<<1
MBOOT_HEADER_MAGIC  equ 0x1BADB002
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

section .multiboot
    align 4
    dd MBOOT_HEADER_MAGIC
    dd MBOOT_HEADER_FLAGS
    dd MBOOT_CHECKSUM

section .text
[GLOBAL _start]
[GLOBAL gdt_flush] ; EXPORT THIS SO GDT.C CAN SEE IT
[EXTERN kmain]
[EXTERN gp]       ; This is the GDT pointer from your gdt.c

_start:
    mov esp, stack_top
    call kmain
    cli
.hang:
    hlt
    jmp .hang

; --- The function the Linker is crying about ---
gdt_flush:
    lgdt [gp]        ; Load the GDT with the pointer 'gp' from gdt.c
    mov ax, 0x10     ; 0x10 is the offset to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush  ; 0x08 is the offset to our code segment, far jump
.flush:
    ret

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .note.GNU-stack noalloc noexec nowrite progbits