#include "common.h"
#include "system.h"

static char last_char = 0;
static int has_new_key = 0;

/* This table translates hardware scancodes to ASCII */
unsigned char kbd_us[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

char scancode_to_ascii(uint8_t scancode) {
    if (scancode >= 128) return 0;
    return kbd_us[scancode];
}

/* 1. THE HANDLER: What happens when a key is pressed */
void keyboard_handler(struct regs *r) {
    (void)r;
    uint8_t scancode = inb(0x60);
    if (!(scancode & 0x80)) {
        last_char = scancode_to_ascii(scancode);
        has_new_key = 1;
    }
}

/* 2. THE INSTALLER: The function your kmain was missing! */
void keyboard_install() {
    /* 
       This tells the IDT: "When Interrupt 33 (IRQ1) happens, 
       run the keyboard_handler function."
    */
    idt_set_gate(33, (uint32_t)keyboard_handler, 0x08, 0x8E);
    print("Keyboard driver initialized.\n");
}

/* 3. THE GETTER: How the shell waits for your input */
char keyboard_getchar() {
    while (!has_new_key); // Loop until handler sets this to 1
    has_new_key = 0;
    return last_char;
}