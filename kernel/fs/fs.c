#include <stdint.h>
#include <stddef.h>

#define MAX_NODES 64

struct node {
    char name[32];
    int is_directory;
    struct node* parent;
};

struct node ram_disk[MAX_NODES];
int node_count = 0;
struct node* current_directory;

void fs_init() {
    // Create Root
    current_directory = &ram_disk[0];
    // Simple logic to add "root"
    node_count = 1;
}

void mkdir(char* name) {
    if (node_count < MAX_NODES) {
        // Simple copy name and set parent to current_directory
        node_count++;
    }
}