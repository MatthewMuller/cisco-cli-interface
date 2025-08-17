#include "cisco_cli.h"

dir_node_t *file_tree_create(const char *name, const char *path, file_type_t type) {
    dir_node_t *node = malloc(sizeof(dir_node_t));
    if (!node) return NULL;
    
    strncpy(node->name, name, MAX_PATH_LEN - 1);
    node->name[MAX_PATH_LEN - 1] = '\0';
    
    strncpy(node->path, path, MAX_PATH_LEN - 1);
    node->path[MAX_PATH_LEN - 1] = '\0';
    
    node->type = type;
    node->selected = 0;
    node->expanded = 0;
    node->level = 0;
    node->parent = NULL;
    node->children = NULL;
    node->next = NULL;
    
    return node;
}

void file_tree_add_child(dir_node_t *parent, dir_node_t *child) {
    if (!parent || !child) return;
    
    child->parent = parent;
    child->level = parent->level + 1;
    
    if (parent->children == NULL) {
        parent->children = child;
    } else {
        dir_node_t *current = parent->children;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = child;
    }
}

void file_tree_build_recursive(serial_conn_t *conn, dir_node_t *parent, const char *path) {
    file_entry_t *files = NULL;
    int file_count = cisco_get_directory_listing(conn, path, &files);
    
    if (file_count <= 0) return;
    
    file_entry_t *current = files;
    while (current != NULL) {
        // Create node for this file/directory
        dir_node_t *node = file_tree_create(current->name, current->full_path, current->type);
        if (node) {
            file_tree_add_child(parent, node);
            
            // Note: We don't recursively explore directories by default
            // to avoid infinite loops. The user can expand directories manually.
        }
        
        file_entry_t *next = current->next;
        free(current);
        current = next;
    }
}

void file_tree_build(serial_conn_t *conn, dir_node_t **root) {
    // Create root node
    *root = file_tree_create("flash:/", "flash:/", FILE_TYPE_DIRECTORY);
    if (!*root) return;
    
    // Build the tree for the root directory only
    file_tree_build_recursive(conn, *root, "flash:/");
}

void file_tree_free(dir_node_t *node) {
    if (!node) return;
    
    // Free children first
    dir_node_t *child = node->children;
    while (child != NULL) {
        dir_node_t *next = child->next;
        file_tree_free(child);
        child = next;
    }
    
    // Free this node
    free(node);
}

void file_tree_select(dir_node_t *node, int selected) {
    if (!node) return;
    
    node->selected = selected;
    
    // If selecting a directory, select all children too
    if (node->type == FILE_TYPE_DIRECTORY) {
        dir_node_t *child = node->children;
        while (child != NULL) {
            file_tree_select(child, selected);
            child = child->next;
        }
    }
}

int file_tree_count_selected(dir_node_t *node) {
    if (!node) return 0;
    
    int count = node->selected ? 1 : 0;
    
    // Count children
    dir_node_t *child = node->children;
    while (child != NULL) {
        count += file_tree_count_selected(child);
        child = child->next;
    }
    
    return count;
}

void file_tree_delete_selected_recursive(serial_conn_t *conn, dir_node_t *node, int *success_count, int *fail_count) {
    if (!node) return;
    
    // Delete children first (for directories)
    if (node->type == FILE_TYPE_DIRECTORY) {
        dir_node_t *child = node->children;
        while (child != NULL) {
            dir_node_t *next = child->next;
            file_tree_delete_selected_recursive(conn, child, success_count, fail_count);
            child = next;
        }
    }
    
    // Delete this node if selected
    if (node->selected) {
        int result;
        
        if (node->type == FILE_TYPE_DIRECTORY) {
            result = cisco_delete_directory(conn, node->path);
        } else {
            result = cisco_delete_file(conn, node->path);
        }
        
        if (result == 0) {
            (*success_count)++;
        } else {
            (*fail_count)++;
        }
    }
}

void file_tree_delete_selected(serial_conn_t *conn, dir_node_t *node) {
    int success_count = 0, fail_count = 0;
    
    file_tree_delete_selected_recursive(conn, node, &success_count, &fail_count);
    
    // Print results
    printf("Deletion complete: %d successful, %d failed\n", success_count, fail_count);
}

// Utility function to get all nodes in a flat list for UI display
int file_tree_get_flat_list_recursive(dir_node_t *node, dir_node_t **list, int max_count, int *current_count) {
    if (!node || *current_count >= max_count) return *current_count;
    
    list[*current_count] = node;
    (*current_count)++;
    
    if (node->expanded) {
        dir_node_t *child = node->children;
        while (child != NULL && *current_count < max_count) {
            file_tree_get_flat_list_recursive(child, list, max_count, current_count);
            child = child->next;
        }
    }
    
    return *current_count;
}

int file_tree_get_flat_list(dir_node_t *node, dir_node_t **list, int max_count) {
    int count = 0;
    return file_tree_get_flat_list_recursive(node, list, max_count, &count);
} 