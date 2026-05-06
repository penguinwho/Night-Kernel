void draw_rect(int x, int y, int w, int h, uint8_t color) {
    uint8_t* screen = (uint8_t*)0xA0000; // Mode 13h address
    for (int i = y; i < y + h; i++) {
        for (int j = x; j < x + w; j++) {
            screen[i * 320 + j] = color;
        }
    }
}

void launch_gui_fm() {
    draw_rect(0, 0, 320, 200, 1);     // Blue background
    draw_rect(20, 20, 280, 160, 7);  // White window
    // Render folder icons...
}