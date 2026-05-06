#include "common.h"
#include "system.h"

void kmain() {
    /* 1. Hardware Initialization */
    gdt_install();
    idt_install();
    keyboard_install();
    // timer_install(); // Uncomment if you want the timer running

    /* 2. Clear BIOS leftovers and set up the screen */
    clear_screen();

    /* 3. CRITICAL: Enable Hardware Interrupts */
    /* Without this, keyboard_getchar() will wait forever because 
       the CPU is ignoring the keyboard's signals. */
    __asm__ __volatile__ ("sti");

    /* 4. Welcome Message */
    print("NIGHT KERNEL v1.0 ONLINE\n");
    print("------------------------\n");
    print("Type something to test the shell...\n\n");

    /* 5. The Main Operational Loop */
    while(1) {
        print("night@root> ");

        /* This function now "blocks" (waits) thanks to our new driver */
        char c = keyboard_getchar();

        /* Print the character the user typed */
        if (c == '\n') {
            print("\n");
        } else if (c == '\b') {
            /* Basic backspace handling if your screen driver supports it */
            print("\b \b"); 
        } else if (c != 0) {
            putchar(c);
            print("\n"); // Move to next line after every key for now
        }
    }
}