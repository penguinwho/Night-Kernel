#include <stdint.h>

// Use the kbd_us map we discussed earlier
unsigned char kbd_us[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

char keyboard_read() {
    uint8_t scancode;
    asm volatile("inb $0x60, %0" : "=a"(scancode));
    if (scancode & 0x80) return 0; // Ignore key release
    return kbd_us[scancode];
}