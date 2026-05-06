#include "../include/common.h"
#include "../include/system.h"

void kmain(void) {
    // 1. Initialize Memory
    gdt_install();
    
    // 2. Initialize Interrupts
    idt_install();
    
    // 3. Start the Hardware Heartbeat
    timer_phase(100); 
    
    // 4. Initialize File System
    fs_init();
    
    // 5. Clear screen and welcome user
    print("NIGHT KERNEL v1.0 ONLINE\n");
    print("------------------------\n");

    // 6. Launch the Shell
    while(1) {
        // This is where your Bash system loop lives
        print("night@root> ");
        // get_input(); ...
    }
}