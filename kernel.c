#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Hardware text mode color constants */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_WHITE = 15,
};

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
uint16_t* terminal_buffer = (uint16_t*) 0xB8000;

void terminal_initialize(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = (uint16_t) ' ' | (uint16_t) 15 << 8;
        }
    }
}

void kmain(void) {
    gdt_install(); // Memory protection initialized
    terminal_initialize();
    
    printf("NIGHT KERNEL v0.1\n");
    printf("GDT Loaded... Memory Protection: ON\n");
}