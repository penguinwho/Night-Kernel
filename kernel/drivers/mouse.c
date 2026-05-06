#include "../../include/common.h"
#include "../../include/mouse.h"

uint8_t mouse_cycle = 0;
int8_t mouse_byte[3];
int mouse_x = 160, mouse_y = 100; // Screen center for 320x200

void mouse_wait(uint8_t type) {
    uint32_t timeout = 100000;
    if (type == 0) {
        while (timeout--) { if ((inb(0x64) & 1) == 1) return; }
    } else {
        while (timeout--) { if ((inb(0x64) & 2) == 0) return; }
    }
}

void mouse_write(uint8_t write) {
    mouse_wait(1);
    outb(0x64, 0xD4);
    mouse_wait(1);
    outb(0x60, write);
}

uint8_t mouse_read() {
    mouse_wait(0);
    return inb(0x60);
}

void mouse_install() {
    uint8_t status;
    mouse_wait(1);
    outb(0x64, 0xA8); // Enable mouse
    mouse_wait(1);
    outb(0x64, 0x20); // Get status
    mouse_wait(0);
    status = (inb(0x60) | 2);
    mouse_wait(1);
    outb(0x64, 0x60); // Set status
    mouse_wait(1);
    outb(0x60, status);
    mouse_write(0xF4); // Enable data reporting
    mouse_read();
}

void mouse_handler() {
    switch(mouse_cycle) {
        case 0:
            mouse_byte[0] = inb(0x60);
            mouse_cycle++;
            break;
        case 1:
            mouse_byte[1] = inb(0x60);
            mouse_cycle++;
            break;
        case 2:
            mouse_byte[2] = inb(0x60);
            mouse_x += mouse_byte[1];
            mouse_y -= mouse_byte[2];
            // Prevent mouse from leaving screen
            if (mouse_x < 0) mouse_x = 0;
            if (mouse_y < 0) mouse_y = 0;
            if (mouse_x > 319) mouse_x = 319;
            if (mouse_y > 199) mouse_y = 199;
            mouse_cycle = 0;
            break;
    }
}