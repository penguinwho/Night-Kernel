#ifndef SYSTEM_H
#define SYSTEM_H

#include "common.h"

/* --- Standard Hardware Types --- */
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

/* --- Interrupt Register Structure --- */
/* This defines what the stack looks like after an ISR runs */
struct regs {
    uint32_t gs, fs, es, ds;                         /* Data segment selector */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha */
    uint32_t int_no, err_code;                       /* Interrupt number and error code */
    uint32_t eip, cs, eflags, useresp, ss;           /* Pushed by the processor automatically */
};

/* --- Screen Driver (screen.c) --- */
void clear_screen();
void putchar(char c);
void print(const char *text);
void print_hex(uint32_t n);

/* --- Keyboard Driver (keyboard.c) --- */
void keyboard_install();
void keyboard_handler(struct regs *r);
char keyboard_getchar();
char scancode_to_ascii(uint8_t scancode);

/* --- IDT / GDT Logic (arch folder) --- */
void gdt_install();
void idt_install();
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);


#endif