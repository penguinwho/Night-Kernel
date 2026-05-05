struct fs_node* current_dir;
struct fs_node root;

void fs_init() {
    strcpy(root.name, "/");
    root.type = FS_DIRECTORY;
    root.parent = &root;
    current_dir = &root;
}

void make_directory(char* name) {
    struct fs_node* new_dir = malloc(sizeof(struct fs_node));
    strcpy(new_dir->name, name);
    new_dir->type = FS_DIRECTORY;
    new_dir->parent = current_dir;
    
    // Add to current directory's children
    for(int i = 0; i < 10; i++) {
        if(current_dir->children[i] == NULL) {
            current_dir->children[i] = new_dir;
            break;
        }
    }
}