#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* --- SYSTEM CONSTANTS --- */
#define MAX_CHILDREN 10
#define MAX_NAME 32
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

/* --- VFS DATA STRUCTURES --- */
typedef enum { FS_FILE, FS_DIR } NODE_TYPE;

struct fs_node {
    char name[MAX_NAME];
    NODE_TYPE type;
    struct fs_node* parent;
    struct fs_node* children[MAX_CHILDREN];
    int child_count;
    char* content; // For files
};

struct fs_node root = {"root", FS_DIR, &root, {0}, 0, NULL};
struct fs_node* current_dir = &root;

/* --- UTILITIES --- */
void strcpy(char* dest, const char* src) {
    while ((*dest++ = *src++));
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    while (n--) {
        if (*s1 != *s2) return *(unsigned char*)s1 - *(unsigned char*)s2;
        if (*s1 == 0) break;
        s1++; s2++;
    }
    return 0;
}

/* --- VGA DRIVER --- */
uint16_t* terminal_buffer = (uint16_t*) 0xB8000;
int term_x = 0, term_y = 0;

void putchar(char c) {
    if (c == '\n') { term_x = 0; term_y++; }
    else {
        terminal_buffer[term_y * VGA_WIDTH + term_x] = (uint16_t)c | (uint16_t)15 << 8;
        term_x++;
    }
}

void print(const char* s) { while(*s) putchar(*s++); }

/* --- FILE SYSTEM OPERATIONS --- */

// Helper to find a child by name
struct fs_node* find_child(struct fs_node* parent, char* name) {
    for (int i = 0; i < parent->child_count; i++) {
        if (strcmp(parent->children[i]->name, name) == 0) return parent->children[i];
    }
    return NULL;
}

// Simple Static Memory Allocator for Nodes (Simulating Malloc)
struct fs_node node_pool[64];
int pool_ptr = 0;

struct fs_node* create_node(char* name, NODE_TYPE type) {
    struct fs_node* n = &node_pool[pool_ptr++];
    strcpy(n->name, name);
    n->type = type;
    n->parent = current_dir;
    n->child_count = 0;
    return n;
}

/* --- COMMANDS --- */

void do_ls() {
    print("\n");
    for (int i = 0; i < current_dir->child_count; i++) {
        if (current_dir->children[i]->type == FS_DIR) print("[DIR] ");
        else print("[FIL] ");
        print(current_dir->children[i]->name);
        print("  ");
    }
}

void do_mkdir(char* name) {
    if (current_dir->child_count < MAX_CHILDREN) {
        current_dir->children[current_dir->child_count++] = create_node(name, FS_DIR);
    }
}

void do_cd(char* name) {
    if (strcmp(name, "..") == 0) {
        current_dir = current_dir->parent;
    } else {
        struct fs_node* target = find_child(current_dir, name);
        if (target && target->type == FS_DIR) current_dir = target;
        else print("\nDirectory not found.");
    }
}

void do_cp(char* src_name, char* dest_name) {
    struct fs_node* src = find_child(current_dir, src_name);
    if (src && current_dir->child_count < MAX_CHILDREN) {
        struct fs_node* new_node = create_node(dest_name, src->type);
        current_dir->children[current_dir->child_count++] = new_node;
        print("\nCopied.");
    }
}

void do_mv(char* src_name, char* dest_name) {
    struct fs_node* src = find_child(current_dir, src_name);
    if (src) {
        strcpy(src->name, dest_name);
        print("\nRenamed/Moved.");
    }
}

/* --- SHELL & KERNEL MAIN --- */

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

unsigned char kbd[128] = {0,27,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\','z','x','c','v','b','n','m',',','.','/',0,'*',0,' '};

void kmain() {
    print("Welcome to NIGHT KERNEL\nType 'help' for commands.");
    char cmd[64];
    int i = 0;

    while(1) {
        print("\nnight@"); print(current_dir->name); print("> ");
        i = 0;
        while(1) {
            uint8_t sc = inb(0x60);
            if (!(sc & 0x80) && kbd[sc] != 0) {
                char c = kbd[sc];
                if (c == '\n') { cmd[i] = 0; break; }
                else if (i < 63) { cmd[i++] = c; putchar(c); }
                while(!(inb(0x60) & 0x80)); // Wait for release
            }
        }

        if (strcmp(cmd, "ls") == 0) do_ls();
        else if (strncmp(cmd, "mkdir ", 6) == 0) do_mkdir(cmd + 6);
        else if (strncmp(cmd, "cd ", 3) == 0) do_cd(cmd + 3);
        else if (strncmp(cmd, "mv ", 3) == 0) do_mv(cmd + 3, "new_name"); // Simple mv
        else if (strcmp(cmd, "help") == 0) print("\nCommands: ls, mkdir, cd, mv, cp, help");
        else print("\nUnknown Command.");
    }
}