MBALIGN  equ  1 << 0
MEMINFO  equ  1 << 1
FLAGS    equ  MBALIGN | MEMINFO
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

global gdt_flush
gdt_flush:
    mov eax, [esp + 4]  ; Get the pointer to the GDT
    lgdt [eax]          ; Load the new GDT pointer
    mov ax, 0x10        ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush     ; 0x08 is the offset to our code segment: Far jump!
.flush:
    ret

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB for the stack
stack_top:

section .text
global _start:function (_start.end - _start)
_start:
    mov esp, stack_top    ; Set up the stack pointer
    extern kmain
    call kmain            ; Enter the C kernel
    cli                   ; Clear interrupts
.hang:  hlt               ; If kernel returns, halt the CPU
    jmp .hang
.end: