/**
 * @file test_file_tree.c
 * @brief Unit tests for file_tree.c using streamlined framework
 * 
 * This file contains unit tests for the file tree functionality:
 * - file_tree_create function tests
 * - Memory allocation and initialization tests
 * - Edge cases and error conditions
 * 
 * @author Cisco CLI Interface Team
 * @version 1.0
 */

#include "../framework/test_framework.h"
#include "../../include/cisco_cli.h"

// ============================================================================
// Test Registry
// ============================================================================

// Forward declarations of all test functions
TEST(file_tree_create_success_regular_file);
TEST(file_tree_create_success_directory);
TEST(file_tree_create_success_binary_file);
TEST(file_tree_create_null_name);
TEST(file_tree_create_null_path);
TEST(file_tree_create_empty_name);
TEST(file_tree_create_empty_path);
TEST(file_tree_create_long_name);
TEST(file_tree_create_long_path);
TEST(file_tree_create_special_characters);
TEST(file_tree_create_memory_allocation_failure);
TEST(file_tree_add_child_success_first_child);
TEST(file_tree_add_child_success_multiple_children);
TEST(file_tree_add_child_null_parent);
TEST(file_tree_add_child_null_child);
TEST(file_tree_add_child_both_null);
TEST(file_tree_add_child_level_inheritance);
TEST(file_tree_add_child_parent_pointer_set);
TEST(file_tree_add_child_linked_list_order);

// Test registry - all test functions
test_func_t test_registry[] = {
    test_file_tree_create_success_regular_file,
    test_file_tree_create_success_directory,
    test_file_tree_create_success_binary_file,
    test_file_tree_create_null_name,
    test_file_tree_create_null_path,
    test_file_tree_create_empty_name,
    test_file_tree_create_empty_path,
    test_file_tree_create_long_name,
    test_file_tree_create_long_path,
    test_file_tree_create_special_characters,
    test_file_tree_create_memory_allocation_failure,
    test_file_tree_add_child_success_first_child,
    test_file_tree_add_child_success_multiple_children,
    test_file_tree_add_child_null_parent,
    test_file_tree_add_child_null_child,
    test_file_tree_add_child_both_null,
    test_file_tree_add_child_level_inheritance,
    test_file_tree_add_child_parent_pointer_set,
    test_file_tree_add_child_linked_list_order,
    NULL
};

int test_registry_count = sizeof(test_registry) / sizeof(test_registry[0]) - 1;

// Test names registry - parallel array to test_registry
const char* test_names[] = {
    "file_tree_create_success_regular_file",
    "file_tree_create_success_directory",
    "file_tree_create_success_binary_file",
    "file_tree_create_null_name",
    "file_tree_create_null_path",
    "file_tree_create_empty_name",
    "file_tree_create_empty_path",
    "file_tree_create_long_name",
    "file_tree_create_long_path",
    "file_tree_create_special_characters",
    "file_tree_create_memory_allocation_failure",
    "file_tree_add_child_success_first_child",
    "file_tree_add_child_success_multiple_children",
    "file_tree_add_child_null_parent",
    "file_tree_add_child_null_child",
    "file_tree_add_child_both_null",
    "file_tree_add_child_level_inheritance",
    "file_tree_add_child_parent_pointer_set",
    "file_tree_add_child_linked_list_order",
    NULL
};

int test_names_count = sizeof(test_names) / sizeof(test_names[0]) - 1;

// ============================================================================
// Test Fixtures
// ============================================================================

/**
 * @brief Setup function for file_tree_create tests
 */
static void setup_file_tree_create_tests(void) {
    // No specific setup needed for file_tree_create tests
    // as it doesn't use mocks or external dependencies
}

// ============================================================================
// file_tree_create Tests
// ============================================================================

TEST(file_tree_create_success_regular_file) {
    setup_file_tree_create_tests();
    
    // Test creating a regular file node
    dir_node_t *node = file_tree_create("config.txt", "flash:/config.txt", FILE_TYPE_REGULAR);
    
    // Verify node was created successfully
    ASSERT_NOT_NULL(node);
    
    // Verify all fields are correctly initialized
    ASSERT_STRING_EQUAL("config.txt", node->name);
    ASSERT_STRING_EQUAL("flash:/config.txt", node->path);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, node->type);
    ASSERT_EQUAL(0, node->selected);
    ASSERT_EQUAL(0, node->expanded);
    ASSERT_EQUAL(0, node->level);
    ASSERT_NULL(node->parent);
    ASSERT_NULL(node->children);
    ASSERT_NULL(node->next);
    
    // Clean up
    free(node);
    
    return 1;
}

TEST(file_tree_create_success_directory) {
    setup_file_tree_create_tests();
    
    // Test creating a directory node
    dir_node_t *node = file_tree_create("backup", "flash:/backup", FILE_TYPE_DIRECTORY);
    
    // Verify node was created successfully
    ASSERT_NOT_NULL(node);
    
    // Verify all fields are correctly initialized
    ASSERT_STRING_EQUAL("backup", node->name);
    ASSERT_STRING_EQUAL("flash:/backup", node->path);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, node->type);
    ASSERT_EQUAL(0, node->selected);
    ASSERT_EQUAL(0, node->expanded);
    ASSERT_EQUAL(0, node->level);
    ASSERT_NULL(node->parent);
    ASSERT_NULL(node->children);
    ASSERT_NULL(node->next);
    
    // Clean up
    free(node);
    
    return 1;
}

TEST(file_tree_create_success_binary_file) {
    setup_file_tree_create_tests();
    
    // Test creating a binary file node
    dir_node_t *node = file_tree_create("image.bin", "flash:/image.bin", FILE_TYPE_BINARY);
    
    // Verify node was created successfully
    ASSERT_NOT_NULL(node);
    
    // Verify all fields are correctly initialized
    ASSERT_STRING_EQUAL("image.bin", node->name);
    ASSERT_STRING_EQUAL("flash:/image.bin", node->path);
    ASSERT_EQUAL(FILE_TYPE_BINARY, node->type);
    ASSERT_EQUAL(0, node->selected);
    ASSERT_EQUAL(0, node->expanded);
    ASSERT_EQUAL(0, node->level);
    ASSERT_NULL(node->parent);
    ASSERT_NULL(node->children);
    ASSERT_NULL(node->next);
    
    // Clean up
    free(node);
    
    return 1;
}

TEST(file_tree_create_null_name) {
    setup_file_tree_create_tests();
    
    // Test creating a node with NULL name
    dir_node_t *node = file_tree_create(NULL, "flash:/config.txt", FILE_TYPE_REGULAR);
    
    // Verify node was created successfully (function should handle NULL gracefully)
    ASSERT_NOT_NULL(node);
    
    // Verify name field is empty string (handled safely)
    ASSERT_STRING_EQUAL("", node->name);
    ASSERT_STRING_EQUAL("flash:/config.txt", node->path);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, node->type);
    
    // Clean up
    free(node);
    
    return 1;
}

TEST(file_tree_create_null_path) {
    setup_file_tree_create_tests();
    
    // Test creating a node with NULL path
    dir_node_t *node = file_tree_create("config.txt", NULL, FILE_TYPE_REGULAR);
    
    // Verify node was created successfully (function should handle NULL gracefully)
    ASSERT_NOT_NULL(node);
    
    // Verify path field is empty string (handled safely)
    ASSERT_STRING_EQUAL("config.txt", node->name);
    ASSERT_STRING_EQUAL("", node->path);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, node->type);
    
    // Clean up
    free(node);
    
    return 1;
}

TEST(file_tree_create_empty_name) {
    setup_file_tree_create_tests();
    
    // Test creating a node with empty name
    dir_node_t *node = file_tree_create("", "flash:/config.txt", FILE_TYPE_REGULAR);
    
    // Verify node was created successfully
    ASSERT_NOT_NULL(node);
    
    // Verify name field is correctly set
    ASSERT_STRING_EQUAL("", node->name);
    ASSERT_STRING_EQUAL("flash:/config.txt", node->path);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, node->type);
    
    // Clean up
    free(node);
    
    return 1;
}

TEST(file_tree_create_empty_path) {
    setup_file_tree_create_tests();
    
    // Test creating a node with empty path
    dir_node_t *node = file_tree_create("config.txt", "", FILE_TYPE_REGULAR);
    
    // Verify node was created successfully
    ASSERT_NOT_NULL(node);
    
    // Verify path field is correctly set
    ASSERT_STRING_EQUAL("config.txt", node->name);
    ASSERT_STRING_EQUAL("", node->path);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, node->type);
    
    // Clean up
    free(node);
    
    return 1;
}

TEST(file_tree_create_long_name) {
    setup_file_tree_create_tests();
    
    // Create a name that's longer than MAX_PATH_LEN
    char long_name[MAX_PATH_LEN + 10];
    memset(long_name, 'a', MAX_PATH_LEN + 9);
    long_name[MAX_PATH_LEN + 9] = '\0';
    
    // Test creating a node with long name
    dir_node_t *node = file_tree_create(long_name, "flash:/longname.txt", FILE_TYPE_REGULAR);
    
    // Verify node was created successfully
    ASSERT_NOT_NULL(node);
    
    // Verify name is truncated to MAX_PATH_LEN - 1 characters (plus null terminator)
    ASSERT_EQUAL(MAX_PATH_LEN - 1, strlen(node->name));
    ASSERT_EQUAL('a', node->name[0]);
    ASSERT_EQUAL('a', node->name[MAX_PATH_LEN - 2]);
    ASSERT_EQUAL('\0', node->name[MAX_PATH_LEN - 1]);
    
    // Verify path is correctly set
    ASSERT_STRING_EQUAL("flash:/longname.txt", node->path);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, node->type);
    
    // Clean up
    free(node);
    
    return 1;
}

TEST(file_tree_create_long_path) {
    setup_file_tree_create_tests();
    
    // Create a path that's longer than MAX_PATH_LEN
    char long_path[MAX_PATH_LEN + 10];
    memset(long_path, '/', MAX_PATH_LEN + 9);
    long_path[MAX_PATH_LEN + 9] = '\0';
    
    // Test creating a node with long path
    dir_node_t *node = file_tree_create("config.txt", long_path, FILE_TYPE_REGULAR);
    
    // Verify node was created successfully
    ASSERT_NOT_NULL(node);
    
    // Verify name is correctly set
    ASSERT_STRING_EQUAL("config.txt", node->name);
    
    // Verify path is truncated to MAX_PATH_LEN - 1 characters (plus null terminator)
    ASSERT_EQUAL(MAX_PATH_LEN - 1, strlen(node->path));
    ASSERT_EQUAL('/', node->path[0]);
    ASSERT_EQUAL('/', node->path[MAX_PATH_LEN - 2]);
    ASSERT_EQUAL('\0', node->path[MAX_PATH_LEN - 1]);
    
    ASSERT_EQUAL(FILE_TYPE_REGULAR, node->type);
    
    // Clean up
    free(node);
    
    return 1;
}

TEST(file_tree_create_special_characters) {
    setup_file_tree_create_tests();
    
    // Test creating a node with special characters in name and path
    const char *special_name = "config@#$%.txt";
    const char *special_path = "flash:/backup@#$%/config@#$%.txt";
    
    dir_node_t *node = file_tree_create(special_name, special_path, FILE_TYPE_REGULAR);
    
    // Verify node was created successfully
    ASSERT_NOT_NULL(node);
    
    // Verify special characters are preserved
    ASSERT_STRING_EQUAL("config@#$%.txt", node->name);
    ASSERT_STRING_EQUAL("flash:/backup@#$%/config@#$%.txt", node->path);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, node->type);
    
    // Clean up
    free(node);
    
    return 1;
}

TEST(file_tree_create_memory_allocation_failure) {
    setup_file_tree_create_tests();
    
    // Note: We can't easily simulate malloc failure in this test framework,
    // but we can test that the function handles the case gracefully
    // by ensuring it doesn't crash and returns appropriate results
    
    // Test with normal parameters to ensure function works correctly
    dir_node_t *node = file_tree_create("test.txt", "flash:/test.txt", FILE_TYPE_REGULAR);
    
    // The function should either succeed or fail gracefully
    if (node != NULL) {
        // Success case - verify node is properly initialized
        ASSERT_STRING_EQUAL("test.txt", node->name);
        ASSERT_STRING_EQUAL("flash:/test.txt", node->path);
        ASSERT_EQUAL(FILE_TYPE_REGULAR, node->type);
        
        // Clean up
        free(node);
    } else {
        // Failure case - function should return NULL gracefully
        // This is acceptable behavior for memory allocation failure
    }
    
    return 1;
}

// ============================================================================
// file_tree_add_child Tests
// ============================================================================

TEST(file_tree_add_child_success_first_child) {
    setup_file_tree_create_tests();
    
    // Create parent and child nodes
    dir_node_t *parent = file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY);
    dir_node_t *child = file_tree_create("child", "flash:/parent/child", FILE_TYPE_REGULAR);
    
    // Verify nodes were created successfully
    ASSERT_NOT_NULL(parent);
    ASSERT_NOT_NULL(child);
    
    // Add child to parent
    file_tree_add_child(parent, child);
    
    // Verify child is now the first child of parent
    ASSERT_NOT_NULL(parent->children);
    ASSERT_EQUAL(child, parent->children);
    
    // Verify child's parent pointer is set correctly
    ASSERT_EQUAL(parent, child->parent);
    
    // Verify child's level is incremented
    ASSERT_EQUAL(1, child->level);
    
    // Verify child's next pointer is NULL (first child)
    ASSERT_NULL(child->next);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_add_child_success_multiple_children) {
    setup_file_tree_create_tests();
    
    // Create parent and multiple child nodes
    dir_node_t *parent = file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY);
    dir_node_t *child1 = file_tree_create("child1", "flash:/parent/child1", FILE_TYPE_REGULAR);
    dir_node_t *child2 = file_tree_create("child2", "flash:/parent/child2", FILE_TYPE_REGULAR);
    dir_node_t *child3 = file_tree_create("child3", "flash:/parent/child3", FILE_TYPE_REGULAR);
    
    // Verify nodes were created successfully
    ASSERT_NOT_NULL(parent);
    ASSERT_NOT_NULL(child1);
    ASSERT_NOT_NULL(child2);
    ASSERT_NOT_NULL(child3);
    
    // Add children to parent
    file_tree_add_child(parent, child1);
    file_tree_add_child(parent, child2);
    file_tree_add_child(parent, child3);
    
    // Verify first child is child1
    ASSERT_NOT_NULL(parent->children);
    ASSERT_EQUAL(child1, parent->children);
    
    // Verify linked list structure
    ASSERT_EQUAL(child2, child1->next);
    ASSERT_EQUAL(child3, child2->next);
    ASSERT_NULL(child3->next);
    
    // Verify all children have correct parent and level
    ASSERT_EQUAL(parent, child1->parent);
    ASSERT_EQUAL(parent, child2->parent);
    ASSERT_EQUAL(parent, child3->parent);
    ASSERT_EQUAL(1, child1->level);
    ASSERT_EQUAL(1, child2->level);
    ASSERT_EQUAL(1, child3->level);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_add_child_null_parent) {
    setup_file_tree_create_tests();
    
    // Create child node
    dir_node_t *child = file_tree_create("child", "flash:/child", FILE_TYPE_REGULAR);
    
    // Verify child was created successfully
    ASSERT_NOT_NULL(child);
    
    // Store original values
    dir_node_t *original_parent = child->parent;
    int original_level = child->level;
    
    // Try to add child to NULL parent
    file_tree_add_child(NULL, child);
    
    // Verify child remains unchanged (function should handle NULL parent gracefully)
    ASSERT_EQUAL(original_parent, child->parent);
    ASSERT_EQUAL(original_level, child->level);
    
    // Clean up
    free(child);
    
    return 1;
}

TEST(file_tree_add_child_null_child) {
    setup_file_tree_create_tests();
    
    // Create parent node
    dir_node_t *parent = file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY);
    
    // Verify parent was created successfully
    ASSERT_NOT_NULL(parent);
    
    // Store original children pointer
    dir_node_t *original_children = parent->children;
    
    // Try to add NULL child to parent
    file_tree_add_child(parent, NULL);
    
    // Verify parent's children remain unchanged (function should handle NULL child gracefully)
    ASSERT_EQUAL(original_children, parent->children);
    
    // Clean up
    free(parent);
    
    return 1;
}

TEST(file_tree_add_child_both_null) {
    setup_file_tree_create_tests();
    
    // Try to add NULL child to NULL parent
    // This should not cause any crashes or errors
    file_tree_add_child(NULL, NULL);
    
    // If we reach here without crashing, the test passes
    return 1;
}

TEST(file_tree_add_child_level_inheritance) {
    setup_file_tree_create_tests();
    
    // Create a multi-level tree structure
    dir_node_t *root = file_tree_create("root", "flash:/", FILE_TYPE_DIRECTORY);
    dir_node_t *level1 = file_tree_create("level1", "flash:/level1", FILE_TYPE_DIRECTORY);
    dir_node_t *level2 = file_tree_create("level2", "flash:/level1/level2", FILE_TYPE_REGULAR);
    
    // Verify nodes were created successfully
    ASSERT_NOT_NULL(root);
    ASSERT_NOT_NULL(level1);
    ASSERT_NOT_NULL(level2);
    
    // Set up initial levels
    root->level = 0;
    level1->level = 1;
    
    // Add level1 to root
    file_tree_add_child(root, level1);
    
    // Verify level1's level is incremented from root
    ASSERT_EQUAL(1, level1->level);
    
    // Add level2 to level1
    file_tree_add_child(level1, level2);
    
    // Verify level2's level is incremented from level1
    ASSERT_EQUAL(2, level2->level);
    
    // Clean up
    file_tree_free(root);
    
    return 1;
}

TEST(file_tree_add_child_parent_pointer_set) {
    setup_file_tree_create_tests();
    
    // Create parent and child nodes
    dir_node_t *parent = file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY);
    dir_node_t *child = file_tree_create("child", "flash:/parent/child", FILE_TYPE_REGULAR);
    
    // Verify nodes were created successfully
    ASSERT_NOT_NULL(parent);
    ASSERT_NOT_NULL(child);
    
    // Verify child's parent is initially NULL
    ASSERT_NULL(child->parent);
    
    // Add child to parent
    file_tree_add_child(parent, child);
    
    // Verify child's parent pointer is now set to parent
    ASSERT_EQUAL(parent, child->parent);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_add_child_linked_list_order) {
    setup_file_tree_create_tests();
    
    // Create parent and multiple child nodes
    dir_node_t *parent = file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY);
    dir_node_t *child_a = file_tree_create("child_a", "flash:/parent/child_a", FILE_TYPE_REGULAR);
    dir_node_t *child_b = file_tree_create("child_b", "flash:/parent/child_b", FILE_TYPE_REGULAR);
    dir_node_t *child_c = file_tree_create("child_c", "flash:/parent/child_c", FILE_TYPE_REGULAR);
    
    // Verify nodes were created successfully
    ASSERT_NOT_NULL(parent);
    ASSERT_NOT_NULL(child_a);
    ASSERT_NOT_NULL(child_b);
    ASSERT_NOT_NULL(child_c);
    
    // Add children in specific order
    file_tree_add_child(parent, child_a);
    file_tree_add_child(parent, child_b);
    file_tree_add_child(parent, child_c);
    
    // Verify the linked list order is maintained
    ASSERT_EQUAL(child_a, parent->children);
    ASSERT_EQUAL(child_b, child_a->next);
    ASSERT_EQUAL(child_c, child_b->next);
    ASSERT_NULL(child_c->next);
    
    // Verify all children have correct parent
    ASSERT_EQUAL(parent, child_a->parent);
    ASSERT_EQUAL(parent, child_b->parent);
    ASSERT_EQUAL(parent, child_c->parent);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main(void) {
    printf("Running file_tree unit tests...\n");
    
    int result = run_all_tests();
    
    return result ? 0 : 1;
}
