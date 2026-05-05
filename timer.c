void timer_phase(int hz) {
    int divisor = 1193180 / hz;       // PIT frequency is 1.19MHz
    outb(0x43, 0x36);                 // Command byte
    outb(0x40, divisor & 0xFF);       // Low byte
    outb(0x40, (divisor >> 8) & 0xFF);// High byte
}