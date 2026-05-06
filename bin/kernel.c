#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "include/common.h"
#include "include/system.h"

/* --- CONFIG & DATA STRUCTURES --- */
#define MAX_CHILDREN 10
#define MAX_NAME 32
#define VGA_WIDTH 80

// --- PROGRAM DEFINITIONS ---
typedef void (*program_entry_t)(int argc, char** argv);
struct program {
    char name[16];
    program_entry_t entry;
};

void prog_echo(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        print(argv[i]); print(" ");
    }
}

void prog_reboot(int argc, char** argv) {
    print("Rebooting...");
    uint8_t good = 0x02;
    while (good & 0x02) good = inb(0x64);
    outb(0x64, 0xFE); // Pulse the CPU reset line
}

struct program bin[] = {
    {"echo", prog_echo},
    {"reboot", prog_reboot}
};

typedef enum { FS_FILE, FS_DIR } NODE_TYPE;

struct fs_node {
    char name[MAX_NAME];
    NODE_TYPE type;
    struct fs_node* parent;
    struct fs_node* children[MAX_CHILDREN];
    int child_count;
};

/* --- STRING UTILITIES --- */
void strcpy(char* d, const char* s) { while((*d++ = *s++)); }
int strcmp(const char* s1, const char* s2) {
    while(*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}
int strncmp(const char* s1, const char* s2, size_t n) {
    while(n--) {
        if(*s1 != *s2) return *(unsigned char*)s1 - *(unsigned char*)s2;
        if(*s1 == 0) break;
        s1++; s2++;
    }
    return 0;
}

/* --- VFS CORE --- */
struct fs_node node_pool[128]; 
int pool_ptr = 0;
struct fs_node root = {"root", FS_DIR, &root, {0}, 0};
struct fs_node* current_dir = &root;

struct fs_node* create_node(char* name, NODE_TYPE type, struct fs_node* parent) {
    struct fs_node* n = &node_pool[pool_ptr++];
    strcpy(n->name, name);
    n->type = type;
    n->parent = parent;
    n->child_count = 0;
    return n;
}

struct fs_node* find_child(struct fs_node* dir, char* name) {
    for(int i = 0; i < dir->child_count; i++) {
        if(strcmp(dir->children[i]->name, name) == 0) return dir->children[i];
    }
    return NULL;
}

/* --- COMMAND IMPLEMENTATIONS --- */
void do_mkdir(char* name) {
    if(find_child(current_dir, name)) return;
    current_dir->children[current_dir->child_count++] = create_node(name, FS_DIR, current_dir);
}

void do_cd(char* name) {
    if(strcmp(name, "..") == 0) current_dir = current_dir->parent;
    else {
        struct fs_node* target = find_child(current_dir, name);
        if(target && target->type == FS_DIR) current_dir = target;
    }
}

void do_cp(char* src_name, char* dest_name) {
    struct fs_node* src = find_child(current_dir, src_name);
    if(src) {
        current_dir->children[current_dir->child_count++] = create_node(dest_name, src->type, current_dir);
    }
}

void do_mv(char* src_name, char* dest_name) {
    struct fs_node* src = find_child(current_dir, src_name);
    if(src) strcpy(src->name, dest_name);
}

/* --- HARDWARE & SHELL --- */
uint16_t* vid_mem = (uint16_t*) 0xB8000;
int tx = 0, ty = 0;

void putchar(char c) {
    if(c == '\n') { tx = 0; ty++; }
    else { vid_mem[ty * 80 + tx++] = (uint16_t)c | (uint16_t)15 << 8; }
}

void print(const char* s) { while(*s) putchar(*s++); }

void split_command(char* input, char* cmd, char* arg1, char* arg2) {
    int i = 0, j = 0;
    while(input[i] && input[i] != ' ') cmd[j++] = input[i++];
    cmd[j] = 0; j = 0;
    if(input[i]) i++;
    while(input[i] && input[i] != ' ') arg1[j++] = input[i++];
    arg1[j] = 0; j = 0;
    if(input[i]) i++;
    while(input[i]) arg2[j++] = input[i++];
    arg2[j] = 0;
}

void kmain() {
    print("NIGHT KERNEL READY\n");
    char input[64], c1[16], a1[24], a2[24];
    while(1) {
        print("\nnight@"); print(current_dir->name); print("> ");
        // (Keyboard input loop here - same as before)
        // Once input is received:
        split_command(input, c1, a1, a2);
        if(strcmp(c1, "ls") == 0) { /* ls logic */ }
        else if(strcmp(c1, "mkdir") == 0) do_mkdir(a1);
        else if(strcmp(c1, "cd") == 0) do_cd(a1);
        else if(strcmp(c1, "cp") == 0) do_cp(a1, a2);
        else if(strcmp(c1, "mv") == 0) do_mv(a1, a2);
    }
}
void kmain() {
    print("NIGHT KERNEL v1.0\n");
    char input_buffer[64];
    int char_index = 0;

    while(1) {
        print("\nnight@root> ");
        char_index = 0;

        // 1. DATA COLLECTION: Wait for the user to finish a line
        while(1) {
            uint8_t scancode = inb(0x60); // Read from keyboard port
            if (!(scancode & 0x80) && kbd[scancode] != 0) {
                char c = kbd[scancode];
                
                if (c == '\n') { // User pressed Enter
                    input_buffer[char_index] = '\0'; // End the string
                    break;
                } else if (char_index < 63) {
                    input_buffer[char_index++] = c;
                    putchar(c); // Show the character on screen
                }
                while(!(inb(0x60) & 0x80)); // Wait for key release (debounce)
            }
        }

        // 2. COMMAND INTERPRETATION: Decide what to do with the string
        if (strcmp(input_buffer, "ls") == 0) {
            do_ls();
        } else if (strcmp(input_buffer, "reboot") == 0) {
            outb(0x64, 0xFE);
        } else {
            print("\nCommand not found.");
        }
    }
}