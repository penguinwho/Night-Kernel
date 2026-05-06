#ifndef SYSTEM_H
#define SYSTEM_H

// Screen/VGA
void print(const char* s);
void putchar(char c);

// IDT & GDT
void gdt_install();
void idt_install();

// Timer
void timer_phase(int hz);

// File System
void fs_init();
void mkdir(char* name);

// include/system.h
void timer_phase(int hz);
void timer_install();

#endif