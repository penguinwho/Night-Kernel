#include <stdint.h>

// Basic US Keyboard Layout Map
unsigned char kbd_us[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	'9', '0', '-', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0,
 '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0, '*',   0, ' '
};

// Read a byte from a hardware port
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

char get_keypress() {
    static uint8_t last_scancode = 0;
    uint8_t scancode = inb(0x60);
    
    if (scancode != last_scancode) {
        last_scancode = scancode;
        if (scancode & 0x80) {
            // Key released
            return 0;
        } else {
            // Key pressed
            return kbd_us[scancode];
        }
    }
    return 0;
}