#include "system.h"

void clear_screen() {
    /* VGA Text Buffer starts at 0xB8000 */
    volatile uint16_t* vga_buffer = (volatile uint16_t*)0xB8000;
    
    for (int i = 0; i < 80 * 25; i++) {
        /* Space character with Light Gray on Black background */
        vga_buffer[i] = (uint16_t)' ' | (0x0F << 8);
    }
}

void putchar(char c) {
    // Add your existing putchar logic here to print to 0xB8000
}

void print(const char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        putchar(text[i]);
    }
}