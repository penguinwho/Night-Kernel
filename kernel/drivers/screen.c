#include "common.h"

void putchar(char c) {
    // Standard VGA text mode pointer
    volatile uint16_t* vga_buffer = (volatile uint16_t*)0xB8000;
    static int cursor_pos = 0;

    if (c == '\n') {
        cursor_pos += 80 - (cursor_pos % 80);
    } else {
        // 0x0F00 is white text on black background
        vga_buffer[cursor_pos++] = (uint16_t)c | (0x0F << 8);
    }
}

void print(const char* s) {
    for (int i = 0; s[i] != '\0'; i++) {
        putchar(s[i]);
    }
}