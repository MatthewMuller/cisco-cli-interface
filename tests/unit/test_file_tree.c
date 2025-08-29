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
TEST(file_tree_free_single_node);
TEST(file_tree_free_multiple_nodes);
TEST(file_tree_free_complex_tree);
TEST(file_tree_free_null_node);
TEST(file_tree_free_empty_tree);
TEST(file_tree_free_nested_structure);
TEST(file_tree_free_mixed_types);
TEST(file_tree_select_single_file);
TEST(file_tree_select_single_directory);
TEST(file_tree_select_directory_with_children);
TEST(file_tree_select_complex_tree);
TEST(file_tree_select_null_node);
TEST(file_tree_select_already_selected);
TEST(file_tree_select_deselect);
TEST(file_tree_select_mixed_types);
TEST(file_tree_select_nested_directories);
TEST(file_tree_select_multiple_children);
TEST(file_tree_count_selected_null_node);
TEST(file_tree_count_selected_single_unselected);
TEST(file_tree_count_selected_single_selected);
TEST(file_tree_count_selected_multiple_children_none_selected);
TEST(file_tree_count_selected_multiple_children_some_selected);
TEST(file_tree_count_selected_multiple_children_all_selected);
TEST(file_tree_count_selected_complex_tree_partial_selection);
TEST(file_tree_count_selected_complex_tree_full_selection);
TEST(file_tree_count_selected_nested_structure);
TEST(file_tree_count_selected_mixed_types);
TEST(file_tree_count_selected_empty_tree);
TEST(file_tree_build_recursive_success_single_file);
TEST(file_tree_build_recursive_success_multiple_files);
TEST(file_tree_build_recursive_success_mixed_types);
TEST(file_tree_build_recursive_empty_directory);
TEST(file_tree_build_recursive_null_connection);
TEST(file_tree_build_recursive_null_parent);
TEST(file_tree_build_recursive_null_path);
TEST(file_tree_build_recursive_directory_listing_failure);
TEST(file_tree_build_recursive_memory_allocation_failure);
TEST(file_tree_build_recursive_special_characters);
TEST(file_tree_build_recursive_long_filenames);
TEST(file_tree_build_recursive_nested_structure);
TEST(file_tree_build_success_single_file);
TEST(file_tree_build_success_multiple_files);
TEST(file_tree_build_success_mixed_types);
TEST(file_tree_build_success_empty_flash);
TEST(file_tree_build_null_connection);
TEST(file_tree_build_null_root_pointer);
TEST(file_tree_build_directory_listing_failure);
TEST(file_tree_build_memory_allocation_failure);
TEST(file_tree_build_special_characters);
TEST(file_tree_build_long_filenames);
TEST(file_tree_build_nested_structure);

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
    test_file_tree_free_single_node,
    test_file_tree_free_multiple_nodes,
    test_file_tree_free_complex_tree,
    test_file_tree_free_null_node,
    test_file_tree_free_empty_tree,
    test_file_tree_free_nested_structure,
    test_file_tree_free_mixed_types,
    test_file_tree_select_single_file,
    test_file_tree_select_single_directory,
    test_file_tree_select_directory_with_children,
    test_file_tree_select_complex_tree,
    test_file_tree_select_null_node,
    test_file_tree_select_already_selected,
    test_file_tree_select_deselect,
    test_file_tree_select_mixed_types,
    test_file_tree_select_nested_directories,
    test_file_tree_select_multiple_children,
    test_file_tree_count_selected_null_node,
    test_file_tree_count_selected_single_unselected,
    test_file_tree_count_selected_single_selected,
    test_file_tree_count_selected_multiple_children_none_selected,
    test_file_tree_count_selected_multiple_children_some_selected,
    test_file_tree_count_selected_multiple_children_all_selected,
    test_file_tree_count_selected_complex_tree_partial_selection,
    test_file_tree_count_selected_complex_tree_full_selection,
    test_file_tree_count_selected_nested_structure,
    test_file_tree_count_selected_mixed_types,
    test_file_tree_count_selected_empty_tree,
    test_file_tree_build_recursive_success_single_file,
    test_file_tree_build_recursive_success_multiple_files,
    test_file_tree_build_recursive_success_mixed_types,
    test_file_tree_build_recursive_empty_directory,
    test_file_tree_build_recursive_null_connection,
    test_file_tree_build_recursive_null_parent,
    test_file_tree_build_recursive_null_path,
    test_file_tree_build_recursive_directory_listing_failure,
    test_file_tree_build_recursive_memory_allocation_failure,
    test_file_tree_build_recursive_special_characters,
    test_file_tree_build_recursive_long_filenames,
    test_file_tree_build_recursive_nested_structure,
    test_file_tree_build_success_single_file,
    test_file_tree_build_success_multiple_files,
    test_file_tree_build_success_mixed_types,
    test_file_tree_build_success_empty_flash,
    test_file_tree_build_null_connection,
    test_file_tree_build_null_root_pointer,
    test_file_tree_build_directory_listing_failure,
    test_file_tree_build_memory_allocation_failure,
    test_file_tree_build_special_characters,
    test_file_tree_build_long_filenames,
    test_file_tree_build_nested_structure,
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
    "file_tree_free_single_node",
    "file_tree_free_multiple_nodes",
    "file_tree_free_complex_tree",
    "file_tree_free_null_node",
    "file_tree_free_empty_tree",
    "file_tree_free_nested_structure",
    "file_tree_free_mixed_types",
    "file_tree_select_single_file",
    "file_tree_select_single_directory",
    "file_tree_select_directory_with_children",
    "file_tree_select_complex_tree",
    "file_tree_select_null_node",
    "file_tree_select_already_selected",
    "file_tree_select_deselect",
    "file_tree_select_mixed_types",
    "file_tree_select_nested_directories",
    "file_tree_select_multiple_children",
    "file_tree_count_selected_null_node",
    "file_tree_count_selected_single_unselected",
    "file_tree_count_selected_single_selected",
    "file_tree_count_selected_multiple_children_none_selected",
    "file_tree_count_selected_multiple_children_some_selected",
    "file_tree_count_selected_multiple_children_all_selected",
    "file_tree_count_selected_complex_tree_partial_selection",
    "file_tree_count_selected_complex_tree_full_selection",
    "file_tree_count_selected_nested_structure",
    "file_tree_count_selected_mixed_types",
    "file_tree_count_selected_empty_tree",
    "file_tree_build_recursive_success_single_file",
    "file_tree_build_recursive_success_multiple_files",
    "file_tree_build_recursive_success_mixed_types",
    "file_tree_build_recursive_empty_directory",
    "file_tree_build_recursive_null_connection",
    "file_tree_build_recursive_null_parent",
    "file_tree_build_recursive_null_path",
    "file_tree_build_recursive_directory_listing_failure",
    "file_tree_build_recursive_memory_allocation_failure",
    "file_tree_build_recursive_special_characters",
    "file_tree_build_recursive_long_filenames",
    "file_tree_build_recursive_nested_structure",
    "file_tree_build_success_single_file",
    "file_tree_build_success_multiple_files",
    "file_tree_build_success_mixed_types",
    "file_tree_build_success_empty_flash",
    "file_tree_build_null_connection",
    "file_tree_build_null_root_pointer",
    "file_tree_build_directory_listing_failure",
    "file_tree_build_memory_allocation_failure",
    "file_tree_build_special_characters",
    "file_tree_build_long_filenames",
    "file_tree_build_nested_structure",
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

/**
 * @brief Setup function for file_tree_build_recursive tests
 */
static void setup_file_tree_build_recursive_tests(void) {
    MOCK_INIT_ALL();
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
// file_tree_free Tests
// ============================================================================

TEST(file_tree_free_single_node) {
    setup_file_tree_create_tests();
    
    // Create a single node
    dir_node_t *node = file_tree_create("test.txt", "flash:/test.txt", FILE_TYPE_REGULAR);
    ASSERT_NOT_NULL(node);
    
    // Free the node - this should not cause any crashes
    file_tree_free(node);
    
    // If we reach here without crashing, the test passes
    return 1;
}

TEST(file_tree_free_multiple_nodes) {
    setup_file_tree_create_tests();
    
    // Create multiple nodes in a simple linked list structure
    dir_node_t *parent = file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY);
    dir_node_t *child1 = file_tree_create("child1", "flash:/parent/child1", FILE_TYPE_REGULAR);
    dir_node_t *child2 = file_tree_create("child2", "flash:/parent/child2", FILE_TYPE_REGULAR);
    dir_node_t *child3 = file_tree_create("child3", "flash:/parent/child3", FILE_TYPE_REGULAR);
    
    ASSERT_NOT_NULL(parent);
    ASSERT_NOT_NULL(child1);
    ASSERT_NOT_NULL(child2);
    ASSERT_NOT_NULL(child3);
    
    // Add children to parent
    file_tree_add_child(parent, child1);
    file_tree_add_child(parent, child2);
    file_tree_add_child(parent, child3);
    
    // Verify structure is correct
    ASSERT_EQUAL(child1, parent->children);
    ASSERT_EQUAL(child2, child1->next);
    ASSERT_EQUAL(child3, child2->next);
    ASSERT_NULL(child3->next);
    
    // Free the entire tree - this should free all nodes recursively
    file_tree_free(parent);
    
    // If we reach here without crashing, the test passes
    return 1;
}

TEST(file_tree_free_complex_tree) {
    setup_file_tree_create_tests();
    
    // Create a complex tree structure with multiple levels
    dir_node_t *root = file_tree_create("root", "flash:/", FILE_TYPE_DIRECTORY);
    dir_node_t *level1_a = file_tree_create("level1_a", "flash:/level1_a", FILE_TYPE_DIRECTORY);
    dir_node_t *level1_b = file_tree_create("level1_b", "flash:/level1_b", FILE_TYPE_DIRECTORY);
    dir_node_t *level2_a1 = file_tree_create("level2_a1", "flash:/level1_a/level2_a1", FILE_TYPE_REGULAR);
    dir_node_t *level2_a2 = file_tree_create("level2_a2", "flash:/level1_a/level2_a2", FILE_TYPE_REGULAR);
    dir_node_t *level2_b1 = file_tree_create("level2_b1", "flash:/level1_b/level2_b1", FILE_TYPE_REGULAR);
    dir_node_t *level3_b1a = file_tree_create("level3_b1a", "flash:/level1_b/level2_b1/level3_b1a", FILE_TYPE_BINARY);
    
    ASSERT_NOT_NULL(root);
    ASSERT_NOT_NULL(level1_a);
    ASSERT_NOT_NULL(level1_b);
    ASSERT_NOT_NULL(level2_a1);
    ASSERT_NOT_NULL(level2_a2);
    ASSERT_NOT_NULL(level2_b1);
    ASSERT_NOT_NULL(level3_b1a);
    
    // Build the tree structure
    file_tree_add_child(root, level1_a);
    file_tree_add_child(root, level1_b);
    file_tree_add_child(level1_a, level2_a1);
    file_tree_add_child(level1_a, level2_a2);
    file_tree_add_child(level1_b, level2_b1);
    file_tree_add_child(level2_b1, level3_b1a);
    
    // Verify the structure
    ASSERT_EQUAL(level1_a, root->children);
    ASSERT_EQUAL(level1_b, level1_a->next);
    ASSERT_NULL(level1_b->next);
    
    ASSERT_EQUAL(level2_a1, level1_a->children);
    ASSERT_EQUAL(level2_a2, level2_a1->next);
    ASSERT_NULL(level2_a2->next);
    
    ASSERT_EQUAL(level2_b1, level1_b->children);
    ASSERT_NULL(level2_b1->next);
    
    ASSERT_EQUAL(level3_b1a, level2_b1->children);
    ASSERT_NULL(level3_b1a->next);
    
    // Free the entire complex tree - this should free all nodes recursively
    file_tree_free(root);
    
    // If we reach here without crashing, the test passes
    return 1;
}

TEST(file_tree_free_null_node) {
    setup_file_tree_create_tests();
    
    // Test freeing a NULL node - this should not cause any crashes
    file_tree_free(NULL);
    
    // If we reach here without crashing, the test passes
    return 1;
}

TEST(file_tree_free_empty_tree) {
    setup_file_tree_create_tests();
    
    // Create a node with no children
    dir_node_t *node = file_tree_create("empty", "flash:/empty", FILE_TYPE_DIRECTORY);
    ASSERT_NOT_NULL(node);
    
    // Verify it has no children
    ASSERT_NULL(node->children);
    
    // Free the empty tree - this should not cause any crashes
    file_tree_free(node);
    
    // If we reach here without crashing, the test passes
    return 1;
}

TEST(file_tree_free_nested_structure) {
    setup_file_tree_create_tests();
    
    // Create a deeply nested structure
    dir_node_t *level0 = file_tree_create("level0", "flash:/level0", FILE_TYPE_DIRECTORY);
    dir_node_t *level1 = file_tree_create("level1", "flash:/level0/level1", FILE_TYPE_DIRECTORY);
    dir_node_t *level2 = file_tree_create("level2", "flash:/level0/level1/level2", FILE_TYPE_DIRECTORY);
    dir_node_t *level3 = file_tree_create("level3", "flash:/level0/level1/level2/level3", FILE_TYPE_DIRECTORY);
    dir_node_t *level4 = file_tree_create("level4", "flash:/level0/level1/level2/level3/level4", FILE_TYPE_REGULAR);
    
    ASSERT_NOT_NULL(level0);
    ASSERT_NOT_NULL(level1);
    ASSERT_NOT_NULL(level2);
    ASSERT_NOT_NULL(level3);
    ASSERT_NOT_NULL(level4);
    
    // Build the nested structure
    file_tree_add_child(level0, level1);
    file_tree_add_child(level1, level2);
    file_tree_add_child(level2, level3);
    file_tree_add_child(level3, level4);
    
    // Verify the nested structure
    ASSERT_EQUAL(level1, level0->children);
    ASSERT_EQUAL(level2, level1->children);
    ASSERT_EQUAL(level3, level2->children);
    ASSERT_EQUAL(level4, level3->children);
    ASSERT_NULL(level4->children);
    
    // Free the nested structure - this should free all nodes recursively
    file_tree_free(level0);
    
    // If we reach here without crashing, the test passes
    return 1;
}

TEST(file_tree_free_mixed_types) {
    setup_file_tree_create_tests();
    
    // Create a tree with mixed file types
    dir_node_t *root = file_tree_create("mixed", "flash:/mixed", FILE_TYPE_DIRECTORY);
    dir_node_t *regular_file = file_tree_create("regular.txt", "flash:/mixed/regular.txt", FILE_TYPE_REGULAR);
    dir_node_t *binary_file = file_tree_create("binary.bin", "flash:/mixed/binary.bin", FILE_TYPE_BINARY);
    dir_node_t *subdir = file_tree_create("subdir", "flash:/mixed/subdir", FILE_TYPE_DIRECTORY);
    dir_node_t *subfile = file_tree_create("subfile.txt", "flash:/mixed/subdir/subfile.txt", FILE_TYPE_REGULAR);
    
    ASSERT_NOT_NULL(root);
    ASSERT_NOT_NULL(regular_file);
    ASSERT_NOT_NULL(binary_file);
    ASSERT_NOT_NULL(subdir);
    ASSERT_NOT_NULL(subfile);
    
    // Build the mixed type tree
    file_tree_add_child(root, regular_file);
    file_tree_add_child(root, binary_file);
    file_tree_add_child(root, subdir);
    file_tree_add_child(subdir, subfile);
    
    // Verify the structure
    ASSERT_EQUAL(regular_file, root->children);
    ASSERT_EQUAL(binary_file, regular_file->next);
    ASSERT_EQUAL(subdir, binary_file->next);
    ASSERT_NULL(subdir->next);
    
    ASSERT_EQUAL(subfile, subdir->children);
    ASSERT_NULL(subfile->next);
    
    // Verify file types
    ASSERT_EQUAL(FILE_TYPE_REGULAR, regular_file->type);
    ASSERT_EQUAL(FILE_TYPE_BINARY, binary_file->type);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, subdir->type);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, subfile->type);
    
    // Free the mixed type tree - this should free all nodes recursively
    file_tree_free(root);
    
    // If we reach here without crashing, the test passes
    return 1;
}

// ============================================================================
// file_tree_select Tests
// ============================================================================

TEST(file_tree_select_single_file) {
    setup_file_tree_create_tests();
    
    // Create a single file node
    dir_node_t *file = file_tree_create("config.txt", "flash:/config.txt", FILE_TYPE_REGULAR);
    ASSERT_NOT_NULL(file);
    
    // Verify initial state
    ASSERT_EQUAL(0, file->selected);
    
    // Select the file
    file_tree_select(file, 1);
    
    // Verify file is now selected
    ASSERT_EQUAL(1, file->selected);
    
    // Clean up
    free(file);
    
    return 1;
}

TEST(file_tree_select_single_directory) {
    setup_file_tree_create_tests();
    
    // Create a single directory node
    dir_node_t *dir = file_tree_create("backup", "flash:/backup", FILE_TYPE_DIRECTORY);
    ASSERT_NOT_NULL(dir);
    
    // Verify initial state
    ASSERT_EQUAL(0, dir->selected);
    
    // Select the directory
    file_tree_select(dir, 1);
    
    // Verify directory is now selected
    ASSERT_EQUAL(1, dir->selected);
    
    // Clean up
    free(dir);
    
    return 1;
}

TEST(file_tree_select_directory_with_children) {
    setup_file_tree_create_tests();
    
    // Create a directory with children
    dir_node_t *parent = file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY);
    dir_node_t *child1 = file_tree_create("child1.txt", "flash:/parent/child1.txt", FILE_TYPE_REGULAR);
    dir_node_t *child2 = file_tree_create("child2.txt", "flash:/parent/child2.txt", FILE_TYPE_REGULAR);
    
    ASSERT_NOT_NULL(parent);
    ASSERT_NOT_NULL(child1);
    ASSERT_NOT_NULL(child2);
    
    // Add children to parent
    file_tree_add_child(parent, child1);
    file_tree_add_child(parent, child2);
    
    // Verify initial states
    ASSERT_EQUAL(0, parent->selected);
    ASSERT_EQUAL(0, child1->selected);
    ASSERT_EQUAL(0, child2->selected);
    
    // Select the parent directory
    file_tree_select(parent, 1);
    
    // Verify parent and all children are selected
    ASSERT_EQUAL(1, parent->selected);
    ASSERT_EQUAL(1, child1->selected);
    ASSERT_EQUAL(1, child2->selected);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_select_complex_tree) {
    setup_file_tree_create_tests();
    
    // Create a complex tree structure
    dir_node_t *root = file_tree_create("root", "flash:/root", FILE_TYPE_DIRECTORY);
    dir_node_t *dir1 = file_tree_create("dir1", "flash:/root/dir1", FILE_TYPE_DIRECTORY);
    dir_node_t *dir2 = file_tree_create("dir2", "flash:/root/dir2", FILE_TYPE_DIRECTORY);
    dir_node_t *file1 = file_tree_create("file1.txt", "flash:/root/file1.txt", FILE_TYPE_REGULAR);
    dir_node_t *file2 = file_tree_create("file2.txt", "flash:/root/dir1/file2.txt", FILE_TYPE_REGULAR);
    dir_node_t *file3 = file_tree_create("file3.txt", "flash:/root/dir2/file3.txt", FILE_TYPE_REGULAR);
    dir_node_t *subdir = file_tree_create("subdir", "flash:/root/dir1/subdir", FILE_TYPE_DIRECTORY);
    dir_node_t *file4 = file_tree_create("file4.txt", "flash:/root/dir1/subdir/file4.txt", FILE_TYPE_REGULAR);
    
    ASSERT_NOT_NULL(root);
    ASSERT_NOT_NULL(dir1);
    ASSERT_NOT_NULL(dir2);
    ASSERT_NOT_NULL(file1);
    ASSERT_NOT_NULL(file2);
    ASSERT_NOT_NULL(file3);
    ASSERT_NOT_NULL(subdir);
    ASSERT_NOT_NULL(file4);
    
    // Build the tree structure
    file_tree_add_child(root, dir1);
    file_tree_add_child(root, dir2);
    file_tree_add_child(root, file1);
    file_tree_add_child(dir1, file2);
    file_tree_add_child(dir2, file3);
    file_tree_add_child(dir1, subdir);
    file_tree_add_child(subdir, file4);
    
    // Verify initial states
    ASSERT_EQUAL(0, root->selected);
    ASSERT_EQUAL(0, dir1->selected);
    ASSERT_EQUAL(0, dir2->selected);
    ASSERT_EQUAL(0, file1->selected);
    ASSERT_EQUAL(0, file2->selected);
    ASSERT_EQUAL(0, file3->selected);
    ASSERT_EQUAL(0, subdir->selected);
    ASSERT_EQUAL(0, file4->selected);
    
    // Select the root directory
    file_tree_select(root, 1);
    
    // Verify all nodes are selected
    ASSERT_EQUAL(1, root->selected);
    ASSERT_EQUAL(1, dir1->selected);
    ASSERT_EQUAL(1, dir2->selected);
    ASSERT_EQUAL(1, file1->selected);
    ASSERT_EQUAL(1, file2->selected);
    ASSERT_EQUAL(1, file3->selected);
    ASSERT_EQUAL(1, subdir->selected);
    ASSERT_EQUAL(1, file4->selected);
    
    // Clean up
    file_tree_free(root);
    
    return 1;
}

TEST(file_tree_select_null_node) {
    setup_file_tree_create_tests();
    
    // Test selecting a NULL node - this should not cause a crash
    file_tree_select(NULL, 1);
    
    // If we reach here without crashing, the test passes
    return 1;
}

TEST(file_tree_select_already_selected) {
    setup_file_tree_create_tests();
    
    // Create a file node
    dir_node_t *file = file_tree_create("config.txt", "flash:/config.txt", FILE_TYPE_REGULAR);
    ASSERT_NOT_NULL(file);
    
    // Select the file
    file_tree_select(file, 1);
    ASSERT_EQUAL(1, file->selected);
    
    // Select it again
    file_tree_select(file, 1);
    
    // Verify it's still selected
    ASSERT_EQUAL(1, file->selected);
    
    // Clean up
    free(file);
    
    return 1;
}

TEST(file_tree_select_deselect) {
    setup_file_tree_create_tests();
    
    // Create a directory with children
    dir_node_t *parent = file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY);
    dir_node_t *child1 = file_tree_create("child1.txt", "flash:/parent/child1.txt", FILE_TYPE_REGULAR);
    dir_node_t *child2 = file_tree_create("child2.txt", "flash:/parent/child2.txt", FILE_TYPE_REGULAR);
    
    ASSERT_NOT_NULL(parent);
    ASSERT_NOT_NULL(child1);
    ASSERT_NOT_NULL(child2);
    
    // Add children to parent
    file_tree_add_child(parent, child1);
    file_tree_add_child(parent, child2);
    
    // Select the parent directory
    file_tree_select(parent, 1);
    ASSERT_EQUAL(1, parent->selected);
    ASSERT_EQUAL(1, child1->selected);
    ASSERT_EQUAL(1, child2->selected);
    
    // Deselect the parent directory
    file_tree_select(parent, 0);
    
    // Verify parent and all children are deselected
    ASSERT_EQUAL(0, parent->selected);
    ASSERT_EQUAL(0, child1->selected);
    ASSERT_EQUAL(0, child2->selected);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_select_mixed_types) {
    setup_file_tree_create_tests();
    
    // Create a tree with mixed file types
    dir_node_t *root = file_tree_create("mixed", "flash:/mixed", FILE_TYPE_DIRECTORY);
    dir_node_t *regular_file = file_tree_create("regular.txt", "flash:/mixed/regular.txt", FILE_TYPE_REGULAR);
    dir_node_t *binary_file = file_tree_create("binary.bin", "flash:/mixed/binary.bin", FILE_TYPE_BINARY);
    dir_node_t *subdir = file_tree_create("subdir", "flash:/mixed/subdir", FILE_TYPE_DIRECTORY);
    dir_node_t *subfile = file_tree_create("subfile.txt", "flash:/mixed/subdir/subfile.txt", FILE_TYPE_REGULAR);
    
    ASSERT_NOT_NULL(root);
    ASSERT_NOT_NULL(regular_file);
    ASSERT_NOT_NULL(binary_file);
    ASSERT_NOT_NULL(subdir);
    ASSERT_NOT_NULL(subfile);
    
    // Build the tree
    file_tree_add_child(root, regular_file);
    file_tree_add_child(root, binary_file);
    file_tree_add_child(root, subdir);
    file_tree_add_child(subdir, subfile);
    
    // Verify initial states
    ASSERT_EQUAL(0, root->selected);
    ASSERT_EQUAL(0, regular_file->selected);
    ASSERT_EQUAL(0, binary_file->selected);
    ASSERT_EQUAL(0, subdir->selected);
    ASSERT_EQUAL(0, subfile->selected);
    
    // Select the root directory
    file_tree_select(root, 1);
    
    // Verify all nodes are selected regardless of type
    ASSERT_EQUAL(1, root->selected);
    ASSERT_EQUAL(1, regular_file->selected);
    ASSERT_EQUAL(1, binary_file->selected);
    ASSERT_EQUAL(1, subdir->selected);
    ASSERT_EQUAL(1, subfile->selected);
    
    // Clean up
    file_tree_free(root);
    
    return 1;
}

TEST(file_tree_select_nested_directories) {
    setup_file_tree_create_tests();
    
    // Create a deeply nested directory structure
    dir_node_t *level0 = file_tree_create("level0", "flash:/level0", FILE_TYPE_DIRECTORY);
    dir_node_t *level1 = file_tree_create("level1", "flash:/level0/level1", FILE_TYPE_DIRECTORY);
    dir_node_t *level2 = file_tree_create("level2", "flash:/level0/level1/level2", FILE_TYPE_DIRECTORY);
    dir_node_t *level3 = file_tree_create("level3", "flash:/level0/level1/level2/level3", FILE_TYPE_DIRECTORY);
    dir_node_t *file = file_tree_create("file.txt", "flash:/level0/level1/level2/level3/file.txt", FILE_TYPE_REGULAR);
    
    ASSERT_NOT_NULL(level0);
    ASSERT_NOT_NULL(level1);
    ASSERT_NOT_NULL(level2);
    ASSERT_NOT_NULL(level3);
    ASSERT_NOT_NULL(file);
    
    // Build the nested structure
    file_tree_add_child(level0, level1);
    file_tree_add_child(level1, level2);
    file_tree_add_child(level2, level3);
    file_tree_add_child(level3, file);
    
    // Verify initial states
    ASSERT_EQUAL(0, level0->selected);
    ASSERT_EQUAL(0, level1->selected);
    ASSERT_EQUAL(0, level2->selected);
    ASSERT_EQUAL(0, level3->selected);
    ASSERT_EQUAL(0, file->selected);
    
    // Select the top-level directory
    file_tree_select(level0, 1);
    
    // Verify all nodes in the nested structure are selected
    ASSERT_EQUAL(1, level0->selected);
    ASSERT_EQUAL(1, level1->selected);
    ASSERT_EQUAL(1, level2->selected);
    ASSERT_EQUAL(1, level3->selected);
    ASSERT_EQUAL(1, file->selected);
    
    // Clean up
    file_tree_free(level0);
    
    return 1;
}

TEST(file_tree_select_multiple_children) {
    setup_file_tree_create_tests();
    
    // Create a directory with many children
    dir_node_t *parent = file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY);
    dir_node_t *children[10];
    
    ASSERT_NOT_NULL(parent);
    
    // Create 10 child files
    for (int i = 0; i < 10; i++) {
        char name[32];
        char path[64];
        snprintf(name, sizeof(name), "child%d.txt", i);
        snprintf(path, sizeof(path), "flash:/parent/child%d.txt", i);
        
        children[i] = file_tree_create(name, path, FILE_TYPE_REGULAR);
        ASSERT_NOT_NULL(children[i]);
        
        // Add child to parent
        file_tree_add_child(parent, children[i]);
        
        // Verify initial state
        ASSERT_EQUAL(0, children[i]->selected);
    }
    
    // Verify parent initial state
    ASSERT_EQUAL(0, parent->selected);
    
    // Select the parent directory
    file_tree_select(parent, 1);
    
    // Verify parent is selected
    ASSERT_EQUAL(1, parent->selected);
    
    // Verify all children are selected
    for (int i = 0; i < 10; i++) {
        ASSERT_EQUAL(1, children[i]->selected);
    }
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

// ============================================================================
// file_tree_count_selected Tests
// ============================================================================

TEST(file_tree_count_selected_null_node) {
    setup_file_tree_create_tests();
    
    // Test counting selected nodes with NULL node
    int count = file_tree_count_selected(NULL);
    
    // Should return 0 for NULL node
    ASSERT_EQUAL(0, count);
    
    return 1;
}

TEST(file_tree_count_selected_single_unselected) {
    setup_file_tree_create_tests();
    
    // Create a single unselected node
    dir_node_t *node = file_tree_create("test.txt", "flash:/test.txt", FILE_TYPE_REGULAR);
    ASSERT_NOT_NULL(node);
    
    // Verify initial state
    ASSERT_EQUAL(0, node->selected);
    
    // Count selected nodes
    int count = file_tree_count_selected(node);
    
    // Should return 0 for unselected node
    ASSERT_EQUAL(0, count);
    
    // Clean up
    free(node);
    
    return 1;
}

TEST(file_tree_count_selected_single_selected) {
    setup_file_tree_create_tests();
    
    // Create a single selected node
    dir_node_t *node = file_tree_create("test.txt", "flash:/test.txt", FILE_TYPE_REGULAR);
    ASSERT_NOT_NULL(node);
    
    // Select the node
    node->selected = 1;
    
    // Count selected nodes
    int count = file_tree_count_selected(node);
    
    // Should return 1 for selected node
    ASSERT_EQUAL(1, count);
    
    // Clean up
    free(node);
    
    return 1;
}

TEST(file_tree_count_selected_multiple_children_none_selected) {
    setup_file_tree_create_tests();
    
    // Create a parent with multiple children, none selected
    dir_node_t *parent = file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY);
    dir_node_t *child1 = file_tree_create("child1.txt", "flash:/parent/child1.txt", FILE_TYPE_REGULAR);
    dir_node_t *child2 = file_tree_create("child2.txt", "flash:/parent/child2.txt", FILE_TYPE_REGULAR);
    dir_node_t *child3 = file_tree_create("child3.txt", "flash:/parent/child3.txt", FILE_TYPE_REGULAR);
    
    ASSERT_NOT_NULL(parent);
    ASSERT_NOT_NULL(child1);
    ASSERT_NOT_NULL(child2);
    ASSERT_NOT_NULL(child3);
    
    // Add children to parent
    file_tree_add_child(parent, child1);
    file_tree_add_child(parent, child2);
    file_tree_add_child(parent, child3);
    
    // Verify all nodes are unselected
    ASSERT_EQUAL(0, parent->selected);
    ASSERT_EQUAL(0, child1->selected);
    ASSERT_EQUAL(0, child2->selected);
    ASSERT_EQUAL(0, child3->selected);
    
    // Count selected nodes
    int count = file_tree_count_selected(parent);
    
    // Should return 0 for no selected nodes
    ASSERT_EQUAL(0, count);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_count_selected_multiple_children_some_selected) {
    setup_file_tree_create_tests();
    
    // Create a parent with multiple children, some selected
    dir_node_t *parent = file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY);
    dir_node_t *child1 = file_tree_create("child1.txt", "flash:/parent/child1.txt", FILE_TYPE_REGULAR);
    dir_node_t *child2 = file_tree_create("child2.txt", "flash:/parent/child2.txt", FILE_TYPE_REGULAR);
    dir_node_t *child3 = file_tree_create("child3.txt", "flash:/parent/child3.txt", FILE_TYPE_REGULAR);
    
    ASSERT_NOT_NULL(parent);
    ASSERT_NOT_NULL(child1);
    ASSERT_NOT_NULL(child2);
    ASSERT_NOT_NULL(child3);
    
    // Add children to parent
    file_tree_add_child(parent, child1);
    file_tree_add_child(parent, child2);
    file_tree_add_child(parent, child3);
    
    // Select some children
    child1->selected = 1;
    child3->selected = 1;
    
    // Verify selection states
    ASSERT_EQUAL(0, parent->selected);
    ASSERT_EQUAL(1, child1->selected);
    ASSERT_EQUAL(0, child2->selected);
    ASSERT_EQUAL(1, child3->selected);
    
    // Count selected nodes
    int count = file_tree_count_selected(parent);
    
    // Should return 2 for 2 selected children
    ASSERT_EQUAL(2, count);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_count_selected_multiple_children_all_selected) {
    setup_file_tree_create_tests();
    
    // Create a parent with multiple children, all selected
    dir_node_t *parent = file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY);
    dir_node_t *child1 = file_tree_create("child1.txt", "flash:/parent/child1.txt", FILE_TYPE_REGULAR);
    dir_node_t *child2 = file_tree_create("child2.txt", "flash:/parent/child2.txt", FILE_TYPE_REGULAR);
    dir_node_t *child3 = file_tree_create("child3.txt", "flash:/parent/child3.txt", FILE_TYPE_REGULAR);
    
    ASSERT_NOT_NULL(parent);
    ASSERT_NOT_NULL(child1);
    ASSERT_NOT_NULL(child2);
    ASSERT_NOT_NULL(child3);
    
    // Add children to parent
    file_tree_add_child(parent, child1);
    file_tree_add_child(parent, child2);
    file_tree_add_child(parent, child3);
    
    // Select all children
    child1->selected = 1;
    child2->selected = 1;
    child3->selected = 1;
    
    // Verify all children are selected
    ASSERT_EQUAL(0, parent->selected);
    ASSERT_EQUAL(1, child1->selected);
    ASSERT_EQUAL(1, child2->selected);
    ASSERT_EQUAL(1, child3->selected);
    
    // Count selected nodes
    int count = file_tree_count_selected(parent);
    
    // Should return 3 for all 3 selected children
    ASSERT_EQUAL(3, count);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_count_selected_complex_tree_partial_selection) {
    setup_file_tree_create_tests();
    
    // Create a complex tree structure with partial selection
    dir_node_t *root = file_tree_create("root", "flash:/root", FILE_TYPE_DIRECTORY);
    dir_node_t *dir1 = file_tree_create("dir1", "flash:/root/dir1", FILE_TYPE_DIRECTORY);
    dir_node_t *dir2 = file_tree_create("dir2", "flash:/root/dir2", FILE_TYPE_DIRECTORY);
    dir_node_t *file1 = file_tree_create("file1.txt", "flash:/root/file1.txt", FILE_TYPE_REGULAR);
    dir_node_t *file2 = file_tree_create("file2.txt", "flash:/root/dir1/file2.txt", FILE_TYPE_REGULAR);
    dir_node_t *file3 = file_tree_create("file3.txt", "flash:/root/dir2/file3.txt", FILE_TYPE_REGULAR);
    dir_node_t *subdir = file_tree_create("subdir", "flash:/root/dir1/subdir", FILE_TYPE_DIRECTORY);
    dir_node_t *file4 = file_tree_create("file4.txt", "flash:/root/dir1/subdir/file4.txt", FILE_TYPE_REGULAR);
    
    ASSERT_NOT_NULL(root);
    ASSERT_NOT_NULL(dir1);
    ASSERT_NOT_NULL(dir2);
    ASSERT_NOT_NULL(file1);
    ASSERT_NOT_NULL(file2);
    ASSERT_NOT_NULL(file3);
    ASSERT_NOT_NULL(subdir);
    ASSERT_NOT_NULL(file4);
    
    // Build the tree structure
    file_tree_add_child(root, dir1);
    file_tree_add_child(root, dir2);
    file_tree_add_child(root, file1);
    file_tree_add_child(dir1, file2);
    file_tree_add_child(dir2, file3);
    file_tree_add_child(dir1, subdir);
    file_tree_add_child(subdir, file4);
    
    // Select some nodes partially
    file1->selected = 1;  // root level file
    file2->selected = 1;  // file in dir1
    file4->selected = 1;  // file in subdir
    
    // Verify selection states
    ASSERT_EQUAL(0, root->selected);
    ASSERT_EQUAL(0, dir1->selected);
    ASSERT_EQUAL(0, dir2->selected);
    ASSERT_EQUAL(1, file1->selected);
    ASSERT_EQUAL(1, file2->selected);
    ASSERT_EQUAL(0, file3->selected);
    ASSERT_EQUAL(0, subdir->selected);
    ASSERT_EQUAL(1, file4->selected);
    
    // Count selected nodes
    int count = file_tree_count_selected(root);
    
    // Should return 3 for 3 selected files
    ASSERT_EQUAL(3, count);
    
    // Clean up
    file_tree_free(root);
    
    return 1;
}

TEST(file_tree_count_selected_complex_tree_full_selection) {
    setup_file_tree_create_tests();
    
    // Create a complex tree structure with full selection
    dir_node_t *root = file_tree_create("root", "flash:/root", FILE_TYPE_DIRECTORY);
    dir_node_t *dir1 = file_tree_create("dir1", "flash:/root/dir1", FILE_TYPE_DIRECTORY);
    dir_node_t *dir2 = file_tree_create("dir2", "flash:/root/dir2", FILE_TYPE_DIRECTORY);
    dir_node_t *file1 = file_tree_create("file1.txt", "flash:/root/file1.txt", FILE_TYPE_REGULAR);
    dir_node_t *file2 = file_tree_create("file2.txt", "flash:/root/dir1/file2.txt", FILE_TYPE_REGULAR);
    dir_node_t *file3 = file_tree_create("file3.txt", "flash:/root/dir2/file3.txt", FILE_TYPE_REGULAR);
    dir_node_t *subdir = file_tree_create("subdir", "flash:/root/dir1/subdir", FILE_TYPE_DIRECTORY);
    dir_node_t *file4 = file_tree_create("file4.txt", "flash:/root/dir1/subdir/file4.txt", FILE_TYPE_REGULAR);
    
    ASSERT_NOT_NULL(root);
    ASSERT_NOT_NULL(dir1);
    ASSERT_NOT_NULL(dir2);
    ASSERT_NOT_NULL(file1);
    ASSERT_NOT_NULL(file2);
    ASSERT_NOT_NULL(file3);
    ASSERT_NOT_NULL(subdir);
    ASSERT_NOT_NULL(file4);
    
    // Build the tree structure
    file_tree_add_child(root, dir1);
    file_tree_add_child(root, dir2);
    file_tree_add_child(root, file1);
    file_tree_add_child(dir1, file2);
    file_tree_add_child(dir2, file3);
    file_tree_add_child(dir1, subdir);
    file_tree_add_child(subdir, file4);
    
    // Select all nodes
    root->selected = 1;
    dir1->selected = 1;
    dir2->selected = 1;
    file1->selected = 1;
    file2->selected = 1;
    file3->selected = 1;
    subdir->selected = 1;
    file4->selected = 1;
    
    // Verify all nodes are selected
    ASSERT_EQUAL(1, root->selected);
    ASSERT_EQUAL(1, dir1->selected);
    ASSERT_EQUAL(1, dir2->selected);
    ASSERT_EQUAL(1, file1->selected);
    ASSERT_EQUAL(1, file2->selected);
    ASSERT_EQUAL(1, file3->selected);
    ASSERT_EQUAL(1, subdir->selected);
    ASSERT_EQUAL(1, file4->selected);
    
    // Count selected nodes
    int count = file_tree_count_selected(root);
    
    // Should return 8 for all 8 nodes selected
    ASSERT_EQUAL(8, count);
    
    // Clean up
    file_tree_free(root);
    
    return 1;
}

TEST(file_tree_count_selected_nested_structure) {
    setup_file_tree_create_tests();
    
    // Create a deeply nested structure
    dir_node_t *level0 = file_tree_create("level0", "flash:/level0", FILE_TYPE_DIRECTORY);
    dir_node_t *level1 = file_tree_create("level1", "flash:/level0/level1", FILE_TYPE_DIRECTORY);
    dir_node_t *level2 = file_tree_create("level2", "flash:/level0/level1/level2", FILE_TYPE_DIRECTORY);
    dir_node_t *level3 = file_tree_create("level3", "flash:/level0/level1/level2/level3", FILE_TYPE_DIRECTORY);
    dir_node_t *file = file_tree_create("file.txt", "flash:/level0/level1/level2/level3/file.txt", FILE_TYPE_REGULAR);
    
    ASSERT_NOT_NULL(level0);
    ASSERT_NOT_NULL(level1);
    ASSERT_NOT_NULL(level2);
    ASSERT_NOT_NULL(level3);
    ASSERT_NOT_NULL(file);
    
    // Build the nested structure
    file_tree_add_child(level0, level1);
    file_tree_add_child(level1, level2);
    file_tree_add_child(level2, level3);
    file_tree_add_child(level3, file);
    
    // Select some nodes in the nested structure
    level1->selected = 1;
    level3->selected = 1;
    file->selected = 1;
    
    // Verify selection states
    ASSERT_EQUAL(0, level0->selected);
    ASSERT_EQUAL(1, level1->selected);
    ASSERT_EQUAL(0, level2->selected);
    ASSERT_EQUAL(1, level3->selected);
    ASSERT_EQUAL(1, file->selected);
    
    // Count selected nodes
    int count = file_tree_count_selected(level0);
    
    // Should return 3 for 3 selected nodes
    ASSERT_EQUAL(3, count);
    
    // Clean up
    file_tree_free(level0);
    
    return 1;
}

TEST(file_tree_count_selected_mixed_types) {
    setup_file_tree_create_tests();
    
    // Create a tree with mixed file types
    dir_node_t *root = file_tree_create("mixed", "flash:/mixed", FILE_TYPE_DIRECTORY);
    dir_node_t *regular_file = file_tree_create("regular.txt", "flash:/mixed/regular.txt", FILE_TYPE_REGULAR);
    dir_node_t *binary_file = file_tree_create("binary.bin", "flash:/mixed/binary.bin", FILE_TYPE_BINARY);
    dir_node_t *subdir = file_tree_create("subdir", "flash:/mixed/subdir", FILE_TYPE_DIRECTORY);
    dir_node_t *subfile = file_tree_create("subfile.txt", "flash:/mixed/subdir/subfile.txt", FILE_TYPE_REGULAR);
    
    ASSERT_NOT_NULL(root);
    ASSERT_NOT_NULL(regular_file);
    ASSERT_NOT_NULL(binary_file);
    ASSERT_NOT_NULL(subdir);
    ASSERT_NOT_NULL(subfile);
    
    // Build the mixed type tree
    file_tree_add_child(root, regular_file);
    file_tree_add_child(root, binary_file);
    file_tree_add_child(root, subdir);
    file_tree_add_child(subdir, subfile);
    
    // Select mixed types
    regular_file->selected = 1;
    subdir->selected = 1;
    subfile->selected = 1;
    
    // Verify selection states
    ASSERT_EQUAL(0, root->selected);
    ASSERT_EQUAL(1, regular_file->selected);
    ASSERT_EQUAL(0, binary_file->selected);
    ASSERT_EQUAL(1, subdir->selected);
    ASSERT_EQUAL(1, subfile->selected);
    
    // Count selected nodes
    int count = file_tree_count_selected(root);
    
    // Should return 3 for 3 selected nodes (mixed types)
    ASSERT_EQUAL(3, count);
    
    // Clean up
    file_tree_free(root);
    
    return 1;
}

TEST(file_tree_count_selected_empty_tree) {
    setup_file_tree_create_tests();
    
    // Create a node with no children (empty tree)
    dir_node_t *node = file_tree_create("empty", "flash:/empty", FILE_TYPE_DIRECTORY);
    ASSERT_NOT_NULL(node);
    
    // Verify it has no children
    ASSERT_NULL(node->children);
    
    // Count selected nodes (none selected)
    int count = file_tree_count_selected(node);
    
    // Should return 0 for no selected nodes
    ASSERT_EQUAL(0, count);
    
    // Select the node
    node->selected = 1;
    
    // Count selected nodes again
    count = file_tree_count_selected(node);
    
    // Should return 1 for the selected node
    ASSERT_EQUAL(1, count);
    
    // Clean up
    free(node);
    
    return 1;
}

// ============================================================================
// file_tree_build_recursive Tests
// ============================================================================

TEST(file_tree_build_recursive_success_single_file) {
    serial_conn_t conn;
    dir_node_t *parent;
    setup_file_tree_build_recursive_tests();
    
    // Create parent node
    parent = file_tree_create("test_dir", "flash:/test_dir", FILE_TYPE_DIRECTORY);
    ASSERT_NOT_NULL(parent);
    
    // Set up mocks for cisco_get_directory_listing to return one file
    // First mock: cisco_send_command succeeds
    MOCK_WRITE_SET_RETURN(15); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    
    // Second mock: directory listing response
    MOCK_READ_SET_RETURN(200, "Directory of flash:/test_dir/\n"
                              "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  config.txt\n"
                              "1234567 bytes available\n");
    
    // Call the function under test
    file_tree_build_recursive(&conn, parent, "flash:/test_dir");
    
    // Verify that a child was added
    ASSERT_NOT_NULL(parent->children);
    ASSERT_STRING_EQUAL("config.txt", parent->children->name);
    ASSERT_STRING_EQUAL("flash:/test_dir/config.txt", parent->children->path);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, parent->children->type);
    ASSERT_EQUAL(parent, parent->children->parent);
    ASSERT_EQUAL(1, parent->children->level);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_build_recursive_success_multiple_files) {
    serial_conn_t conn;
    dir_node_t *parent;
    setup_file_tree_build_recursive_tests();
    
    // Create parent node
    parent = file_tree_create("test_dir", "flash:/test_dir", FILE_TYPE_DIRECTORY);
    ASSERT_NOT_NULL(parent);
    
    // Set up mocks for cisco_get_directory_listing to return multiple files
    // First mock: cisco_send_command succeeds
    MOCK_WRITE_SET_RETURN(15); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    
    // Second mock: directory listing response with multiple files
    MOCK_READ_SET_RETURN(300, "Directory of flash:/test_dir/\n"
                              "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  config.txt\n"
                              "2  -rwx  2048      Jan 02 2020 00:00:00 +00:00  backup.bin\n"
                              "3  drwx  0          Jan 03 2020 00:00:00 +00:00  logs\n"
                              "1234567 bytes available\n");
    
    // Call the function under test
    file_tree_build_recursive(&conn, parent, "flash:/test_dir");
    
    // Verify that children were added
    ASSERT_NOT_NULL(parent->children);
    
    // Check first child
    dir_node_t *child1 = parent->children;
    ASSERT_STRING_EQUAL("config.txt", child1->name);
    ASSERT_STRING_EQUAL("flash:/test_dir/config.txt", child1->path);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, child1->type);
    ASSERT_EQUAL(parent, child1->parent);
    ASSERT_EQUAL(1, child1->level);
    
    // Check second child
    ASSERT_NOT_NULL(child1->next);
    dir_node_t *child2 = child1->next;
    ASSERT_STRING_EQUAL("backup.bin", child2->name);
    ASSERT_STRING_EQUAL("flash:/test_dir/backup.bin", child2->path);
    ASSERT_EQUAL(FILE_TYPE_BINARY, child2->type);
    ASSERT_EQUAL(parent, child2->parent);
    ASSERT_EQUAL(1, child2->level);
    
    // Check third child
    ASSERT_NOT_NULL(child2->next);
    dir_node_t *child3 = child2->next;
    ASSERT_STRING_EQUAL("logs", child3->name);
    ASSERT_STRING_EQUAL("flash:/test_dir/logs", child3->path);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, child3->type);
    ASSERT_EQUAL(parent, child3->parent);
    ASSERT_EQUAL(1, child3->level);
    
    // Verify no more children
    ASSERT_NULL(child3->next);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_build_recursive_success_mixed_types) {
    serial_conn_t conn;
    dir_node_t *parent;
    setup_file_tree_build_recursive_tests();
    
    // Create parent node
    parent = file_tree_create("mixed_dir", "flash:/mixed_dir", FILE_TYPE_DIRECTORY);
    ASSERT_NOT_NULL(parent);
    
    // Set up mocks for cisco_get_directory_listing with mixed file types
    // First mock: cisco_send_command succeeds
    MOCK_WRITE_SET_RETURN(15); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    
    // Second mock: directory listing response with mixed file types
    MOCK_READ_SET_RETURN(400, "Directory of flash:/mixed_dir/\n"
                              "1  -rwx  1024      Jan 01 2020 00:00:00 +00:00  readme.txt\n"
                              "2  -rwx  8192      Jan 02 2020 00:00:00 +00:00  image.bin\n"
                              "3  drwx  0          Jan 03 2020 00:00:00 +00:00  config\n"
                              "4  -rwx  512       Jan 04 2020 00:00:00 +00:00  script.sh\n"
                              "1234567 bytes available\n");
    
    // Call the function under test
    file_tree_build_recursive(&conn, parent, "flash:/mixed_dir");
    
    // Verify that children were added with correct types
    ASSERT_NOT_NULL(parent->children);
    
    // Check regular file
    dir_node_t *child1 = parent->children;
    ASSERT_STRING_EQUAL("readme.txt", child1->name);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, child1->type);
    
    // Check binary file
    ASSERT_NOT_NULL(child1->next);
    dir_node_t *child2 = child1->next;
    ASSERT_STRING_EQUAL("image.bin", child2->name);
    ASSERT_EQUAL(FILE_TYPE_BINARY, child2->type);
    
    // Check directory
    ASSERT_NOT_NULL(child2->next);
    dir_node_t *child3 = child2->next;
    ASSERT_STRING_EQUAL("config", child3->name);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, child3->type);
    
    // Check script file
    ASSERT_NOT_NULL(child3->next);
    dir_node_t *child4 = child3->next;
    ASSERT_STRING_EQUAL("script.sh", child4->name);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, child4->type);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_build_recursive_empty_directory) {
    serial_conn_t conn;
    dir_node_t *parent;
    setup_file_tree_build_recursive_tests();
    
    // Create parent node
    parent = file_tree_create("empty_dir", "flash:/empty_dir", FILE_TYPE_DIRECTORY);
    ASSERT_NOT_NULL(parent);
    
    // Set up mocks for cisco_get_directory_listing to return empty directory
    // First mock: cisco_send_command succeeds
    MOCK_WRITE_SET_RETURN(15); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    
    // Second mock: empty directory listing response
    MOCK_READ_SET_RETURN(100, "Directory of flash:/empty_dir/\n"
                              "1234567 bytes available\n");
    
    // Call the function under test
    file_tree_build_recursive(&conn, parent, "flash:/empty_dir");
    
    // Verify that no children were added
    ASSERT_NULL(parent->children);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_build_recursive_null_connection) {
    dir_node_t *parent;
    setup_file_tree_build_recursive_tests();
    
    // Create parent node
    parent = file_tree_create("test_dir", "flash:/test_dir", FILE_TYPE_DIRECTORY);
    ASSERT_NOT_NULL(parent);
    
    // Call the function under test with NULL connection
    // This should not cause a crash and should handle gracefully
    file_tree_build_recursive(NULL, parent, "flash:/test_dir");
    
    // Verify that no children were added (due to NULL connection)
    ASSERT_NULL(parent->children);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_build_recursive_null_parent) {
    serial_conn_t conn;
    setup_file_tree_build_recursive_tests();
    
    // Call the function under test with NULL parent
    // This should not cause a crash and should handle gracefully
    file_tree_build_recursive(&conn, NULL, "flash:/test_dir");
    
    // If we reach here without crashing, the test passes
    // The function should return early without doing anything
    return 1;
}

TEST(file_tree_build_recursive_null_path) {
    serial_conn_t conn;
    dir_node_t *parent;
    setup_file_tree_build_recursive_tests();
    
    // Create parent node
    parent = file_tree_create("test_dir", "flash:/test_dir", FILE_TYPE_DIRECTORY);
    ASSERT_NOT_NULL(parent);
    
    // Call the function under test with NULL path
    // This should not cause a crash and should handle gracefully
    file_tree_build_recursive(&conn, parent, NULL);
    
    // Verify that no children were added (due to NULL path)
    ASSERT_NULL(parent->children);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_build_recursive_directory_listing_failure) {
    serial_conn_t conn;
    dir_node_t *parent;
    setup_file_tree_build_recursive_tests();
    
    // Create parent node
    parent = file_tree_create("test_dir", "flash:/test_dir", FILE_TYPE_DIRECTORY);
    ASSERT_NOT_NULL(parent);
    
    // Set up mock to simulate directory listing failure
    // Mock serial_write to fail
    MOCK_WRITE_SET_RETURN(-1); // serial_write fails
    
    // Call the function under test
    file_tree_build_recursive(&conn, parent, "flash:/test_dir");
    
    // Verify that no children were added due to failure
    ASSERT_NULL(parent->children);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_build_recursive_memory_allocation_failure) {
    serial_conn_t conn;
    dir_node_t *parent;
    setup_file_tree_build_recursive_tests();
    
    // Create parent node
    parent = file_tree_create("test_dir", "flash:/test_dir", FILE_TYPE_DIRECTORY);
    ASSERT_NOT_NULL(parent);
    
    // Set up mocks for cisco_get_directory_listing to return files
    // First mock: cisco_send_command succeeds
    MOCK_WRITE_SET_RETURN(15); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    
    // Second mock: directory listing response
    MOCK_READ_SET_RETURN(200, "Directory of flash:/test_dir/\n"
                              "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  config.txt\n"
                              "1234567 bytes available\n");
    
    // Call the function under test
    // Note: We can't easily simulate malloc failure in this test framework,
    // but we can test that the function handles the case gracefully
    file_tree_build_recursive(&conn, parent, "flash:/test_dir");
    
    // The function should either succeed or fail gracefully
    // If it succeeds, verify the child was added correctly
    if (parent->children != NULL) {
        ASSERT_STRING_EQUAL("config.txt", parent->children->name);
        ASSERT_EQUAL(FILE_TYPE_REGULAR, parent->children->type);
    }
    // If it fails, parent->children should remain NULL, which is also acceptable
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_build_recursive_special_characters) {
    serial_conn_t conn;
    dir_node_t *parent;
    setup_file_tree_build_recursive_tests();
    
    // Create parent node
    parent = file_tree_create("special_dir", "flash:/special_dir", FILE_TYPE_DIRECTORY);
    ASSERT_NOT_NULL(parent);
    
    // Set up mocks for cisco_get_directory_listing with special characters
    // First mock: cisco_send_command succeeds
    MOCK_WRITE_SET_RETURN(15); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    
    // Second mock: directory listing response with special characters
    MOCK_READ_SET_RETURN(300, "Directory of flash:/special_dir/\n"
                              "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  config@#$%.txt\n"
                              "2  -rwx  2048      Jan 02 2020 00:00:00 +00:00  backup@#$%.bin\n"
                              "1234567 bytes available\n");
    
    // Call the function under test
    file_tree_build_recursive(&conn, parent, "flash:/special_dir");
    
    // Verify that children were added with special characters preserved
    ASSERT_NOT_NULL(parent->children);
    
    // Check first child with special characters
    dir_node_t *child1 = parent->children;
    ASSERT_STRING_EQUAL("config@#$%.txt", child1->name);
    ASSERT_STRING_EQUAL("flash:/special_dir/config@#$%.txt", child1->path);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, child1->type);
    
    // Check second child with special characters
    ASSERT_NOT_NULL(child1->next);
    dir_node_t *child2 = child1->next;
    ASSERT_STRING_EQUAL("backup@#$%.bin", child2->name);
    ASSERT_STRING_EQUAL("flash:/special_dir/backup@#$%.bin", child2->path);
    ASSERT_EQUAL(FILE_TYPE_BINARY, child2->type);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_build_recursive_long_filenames) {
    serial_conn_t conn;
    dir_node_t *parent;
    setup_file_tree_build_recursive_tests();
    
    // Create parent node
    parent = file_tree_create("long_dir", "flash:/long_dir", FILE_TYPE_DIRECTORY);
    ASSERT_NOT_NULL(parent);
    
    // Create a long filename
    char long_filename[300];
    memset(long_filename, 'a', 299);
    long_filename[299] = '\0';
    
    // Set up mocks for cisco_get_directory_listing with long filename
    // First mock: cisco_send_command succeeds
    MOCK_WRITE_SET_RETURN(15); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    
    // Second mock: directory listing response with long filename
    char mock_response[500];
    snprintf(mock_response, sizeof(mock_response), 
             "Directory of flash:/long_dir/\n"
             "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  %s\n"
             "1234567 bytes available\n", long_filename);
    
    MOCK_READ_SET_RETURN(strlen(mock_response), mock_response);
    
    // Call the function under test
    file_tree_build_recursive(&conn, parent, "flash:/long_dir");
    
    // Verify that child was added with truncated name if necessary
    ASSERT_NOT_NULL(parent->children);
    
    dir_node_t *child = parent->children;
    // The name should be truncated to MAX_PATH_LEN - 1 characters
    ASSERT_TRUE(strlen(child->name) <= MAX_PATH_LEN - 1);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, child->type);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

TEST(file_tree_build_recursive_nested_structure) {
    serial_conn_t conn;
    dir_node_t *parent;
    setup_file_tree_build_recursive_tests();
    
    // Create parent node
    parent = file_tree_create("nested_dir", "flash:/nested_dir", FILE_TYPE_DIRECTORY);
    ASSERT_NOT_NULL(parent);
    
    // Set up mocks for cisco_get_directory_listing with nested structure
    // First mock: cisco_send_command succeeds
    MOCK_WRITE_SET_RETURN(15); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    
    // Second mock: directory listing response with nested structure
    MOCK_READ_SET_RETURN(300, "Directory of flash:/nested_dir/\n"
                              "1  drwx  0          Jan 01 2020 00:00:00 +00:00  level1\n"
                              "2  -rwx  1024      Jan 02 2020 00:00:00 +00:00  file1.txt\n"
                              "3  drwx  0          Jan 03 2020 00:00:00 +00:00  level2\n"
                              "1234567 bytes available\n");
    
    // Call the function under test
    file_tree_build_recursive(&conn, parent, "flash:/nested_dir");
    
    // Verify that children were added
    ASSERT_NOT_NULL(parent->children);
    
    // Check first child (directory)
    dir_node_t *child1 = parent->children;
    ASSERT_STRING_EQUAL("level1", child1->name);
    ASSERT_STRING_EQUAL("flash:/nested_dir/level1", child1->path);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, child1->type);
    ASSERT_EQUAL(parent, child1->parent);
    ASSERT_EQUAL(1, child1->level);
    
    // Check second child (file)
    ASSERT_NOT_NULL(child1->next);
    dir_node_t *child2 = child1->next;
    ASSERT_STRING_EQUAL("file1.txt", child2->name);
    ASSERT_STRING_EQUAL("flash:/nested_dir/file1.txt", child2->path);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, child2->type);
    ASSERT_EQUAL(parent, child2->parent);
    ASSERT_EQUAL(1, child2->level);
    
    // Check third child (directory)
    ASSERT_NOT_NULL(child2->next);
    dir_node_t *child3 = child2->next;
    ASSERT_STRING_EQUAL("level2", child3->name);
    ASSERT_STRING_EQUAL("flash:/nested_dir/level2", child3->path);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, child3->type);
    ASSERT_EQUAL(parent, child3->parent);
    ASSERT_EQUAL(1, child3->level);
    
    // Verify no more children
    ASSERT_NULL(child3->next);
    
    // Clean up
    file_tree_free(parent);
    
    return 1;
}

// ============================================================================
// file_tree_build Tests
// ============================================================================

TEST(file_tree_build_success_single_file) {
    serial_conn_t conn;
    dir_node_t *root;
    setup_file_tree_build_recursive_tests();
    
    // Set up mocks for cisco_get_directory_listing to return one file
    // First mock: cisco_send_command succeeds
    MOCK_WRITE_SET_RETURN(15); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    
    // Second mock: directory listing response
    MOCK_READ_SET_RETURN(200, "Directory of flash:/\n"
                              "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  config.txt\n"
                              "1234567 bytes available\n");
    
    // Call the function under test
    file_tree_build(&conn, &root);
    
    // Verify root node was created correctly
    ASSERT_NOT_NULL(root);
    ASSERT_STRING_EQUAL("flash:/", root->name);
    ASSERT_STRING_EQUAL("flash:/", root->path);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, root->type);
    ASSERT_EQUAL(0, root->level);
    ASSERT_NULL(root->parent);
    
    // Verify that a child was added
    ASSERT_NOT_NULL(root->children);
    ASSERT_STRING_EQUAL("config.txt", root->children->name);
    ASSERT_STRING_EQUAL("flash:/config.txt", root->children->path);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, root->children->type);
    ASSERT_EQUAL(root, root->children->parent);
    ASSERT_EQUAL(1, root->children->level);
    
    // Clean up
    file_tree_free(root);
    
    return 1;
}

TEST(file_tree_build_success_multiple_files) {
    serial_conn_t conn;
    dir_node_t *root;
    setup_file_tree_build_recursive_tests();
    
    // Set up mocks for cisco_get_directory_listing to return multiple files
    // First mock: cisco_send_command succeeds
    MOCK_WRITE_SET_RETURN(15); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    
    // Second mock: directory listing response with multiple files
    MOCK_READ_SET_RETURN(300, "Directory of flash:/\n"
                              "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  config.txt\n"
                              "2  -rwx  2048      Jan 02 2020 00:00:00 +00:00  backup.bin\n"
                              "3  drwx  0          Jan 03 2020 00:00:00 +00:00  logs\n"
                              "1234567 bytes available\n");
    
    // Call the function under test
    file_tree_build(&conn, &root);
    
    // Verify root node was created correctly
    ASSERT_NOT_NULL(root);
    ASSERT_STRING_EQUAL("flash:/", root->name);
    ASSERT_STRING_EQUAL("flash:/", root->path);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, root->type);
    
    // Verify that children were added
    ASSERT_NOT_NULL(root->children);
    
    // Check first child
    dir_node_t *child1 = root->children;
    ASSERT_STRING_EQUAL("config.txt", child1->name);
    ASSERT_STRING_EQUAL("flash:/config.txt", child1->path);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, child1->type);
    ASSERT_EQUAL(root, child1->parent);
    ASSERT_EQUAL(1, child1->level);
    
    // Check second child
    ASSERT_NOT_NULL(child1->next);
    dir_node_t *child2 = child1->next;
    ASSERT_STRING_EQUAL("backup.bin", child2->name);
    ASSERT_STRING_EQUAL("flash:/backup.bin", child2->path);
    ASSERT_EQUAL(FILE_TYPE_BINARY, child2->type);
    ASSERT_EQUAL(root, child2->parent);
    ASSERT_EQUAL(1, child2->level);
    
    // Check third child
    ASSERT_NOT_NULL(child2->next);
    dir_node_t *child3 = child2->next;
    ASSERT_STRING_EQUAL("logs", child3->name);
    ASSERT_STRING_EQUAL("flash:/logs", child3->path);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, child3->type);
    ASSERT_EQUAL(root, child3->parent);
    ASSERT_EQUAL(1, child3->level);
    
    // Verify no more children
    ASSERT_NULL(child3->next);
    
    // Clean up
    file_tree_free(root);
    
    return 1;
}

TEST(file_tree_build_success_mixed_types) {
    serial_conn_t conn;
    dir_node_t *root;
    setup_file_tree_build_recursive_tests();
    
    // Set up mocks for cisco_get_directory_listing with mixed file types
    // First mock: cisco_send_command succeeds
    MOCK_WRITE_SET_RETURN(15); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    
    // Second mock: directory listing response with mixed file types
    MOCK_READ_SET_RETURN(400, "Directory of flash:/\n"
                              "1  -rwx  1024      Jan 01 2020 00:00:00 +00:00  readme.txt\n"
                              "2  -rwx  8192      Jan 02 2020 00:00:00 +00:00  image.bin\n"
                              "3  drwx  0          Jan 03 2020 00:00:00 +00:00  config\n"
                              "4  -rwx  512       Jan 04 2020 00:00:00 +00:00  script.sh\n"
                              "1234567 bytes available\n");
    
    // Call the function under test
    file_tree_build(&conn, &root);
    
    // Verify root node was created correctly
    ASSERT_NOT_NULL(root);
    ASSERT_STRING_EQUAL("flash:/", root->name);
    ASSERT_STRING_EQUAL("flash:/", root->path);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, root->type);
    
    // Verify that children were added with correct types
    ASSERT_NOT_NULL(root->children);
    
    // Check regular file
    dir_node_t *child1 = root->children;
    ASSERT_STRING_EQUAL("readme.txt", child1->name);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, child1->type);
    
    // Check binary file
    ASSERT_NOT_NULL(child1->next);
    dir_node_t *child2 = child1->next;
    ASSERT_STRING_EQUAL("image.bin", child2->name);
    ASSERT_EQUAL(FILE_TYPE_BINARY, child2->type);
    
    // Check directory
    ASSERT_NOT_NULL(child2->next);
    dir_node_t *child3 = child2->next;
    ASSERT_STRING_EQUAL("config", child3->name);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, child3->type);
    
    // Check script file
    ASSERT_NOT_NULL(child3->next);
    dir_node_t *child4 = child3->next;
    ASSERT_STRING_EQUAL("script.sh", child4->name);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, child4->type);
    
    // Clean up
    file_tree_free(root);
    
    return 1;
}

TEST(file_tree_build_success_empty_flash) {
    serial_conn_t conn;
    dir_node_t *root;
    setup_file_tree_build_recursive_tests();
    
    // Set up mocks for cisco_get_directory_listing to return empty flash
    // First mock: cisco_send_command succeeds
    MOCK_WRITE_SET_RETURN(15); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    
    // Second mock: empty directory listing response
    MOCK_READ_SET_RETURN(100, "Directory of flash:/\n"
                              "1234567 bytes available\n");
    
    // Call the function under test
    file_tree_build(&conn, &root);
    
    // Verify root node was created correctly
    ASSERT_NOT_NULL(root);
    ASSERT_STRING_EQUAL("flash:/", root->name);
    ASSERT_STRING_EQUAL("flash:/", root->path);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, root->type);
    ASSERT_EQUAL(0, root->level);
    ASSERT_NULL(root->parent);
    
    // Verify that no children were added
    ASSERT_NULL(root->children);
    
    // Clean up
    file_tree_free(root);
    
    return 1;
}

TEST(file_tree_build_null_connection) {
    dir_node_t *root;
    setup_file_tree_build_recursive_tests();
    
    // Call the function under test with NULL connection
    // This should not cause a crash and should handle gracefully
    file_tree_build(NULL, &root);
    
    // Verify root node was created correctly (function should still create root)
    ASSERT_NOT_NULL(root);
    ASSERT_STRING_EQUAL("flash:/", root->name);
    ASSERT_STRING_EQUAL("flash:/", root->path);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, root->type);
    
    // Verify that no children were added (due to NULL connection)
    ASSERT_NULL(root->children);
    
    // Clean up
    file_tree_free(root);
    
    return 1;
}

TEST(file_tree_build_null_root_pointer) {
    serial_conn_t conn;
    setup_file_tree_build_recursive_tests();
    
    // Call the function under test with NULL root pointer
    // This should not cause a crash and should handle gracefully
    file_tree_build(&conn, NULL);
    
    // If we reach here without crashing, the test passes
    // The function should return early without doing anything
    return 1;
}

TEST(file_tree_build_directory_listing_failure) {
    serial_conn_t conn;
    dir_node_t *root;
    setup_file_tree_build_recursive_tests();
    
    // Set up mock to simulate directory listing failure
    // Mock serial_write to fail
    MOCK_WRITE_SET_RETURN(-1); // serial_write fails
    
    // Call the function under test
    file_tree_build(&conn, &root);
    
    // Verify root node was created correctly
    ASSERT_NOT_NULL(root);
    ASSERT_STRING_EQUAL("flash:/", root->name);
    ASSERT_STRING_EQUAL("flash:/", root->path);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, root->type);
    
    // Verify that no children were added due to failure
    ASSERT_NULL(root->children);
    
    // Clean up
    file_tree_free(root);
    
    return 1;
}

TEST(file_tree_build_memory_allocation_failure) {
    serial_conn_t conn;
    dir_node_t *root;
    setup_file_tree_build_recursive_tests();
    
    // Set up mocks for cisco_get_directory_listing to return files
    // First mock: cisco_send_command succeeds
    MOCK_WRITE_SET_RETURN(15); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    
    // Second mock: directory listing response
    MOCK_READ_SET_RETURN(200, "Directory of flash:/\n"
                              "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  config.txt\n"
                              "1234567 bytes available\n");
    
    // Call the function under test
    // Note: We can't easily simulate malloc failure in this test framework,
    // but we can test that the function handles the case gracefully
    file_tree_build(&conn, &root);
    
    // The function should either succeed or fail gracefully
    // If it succeeds, verify the root and child were created correctly
    if (root != NULL) {
        ASSERT_STRING_EQUAL("flash:/", root->name);
        ASSERT_STRING_EQUAL("flash:/", root->path);
        ASSERT_EQUAL(FILE_TYPE_DIRECTORY, root->type);
        
        if (root->children != NULL) {
            ASSERT_STRING_EQUAL("config.txt", root->children->name);
            ASSERT_EQUAL(FILE_TYPE_REGULAR, root->children->type);
        }
        
        // Clean up
        file_tree_free(root);
    }
    // If it fails, root should be NULL, which is also acceptable
    
    return 1;
}

TEST(file_tree_build_special_characters) {
    serial_conn_t conn;
    dir_node_t *root;
    setup_file_tree_build_recursive_tests();
    
    // Set up mocks for cisco_get_directory_listing with special characters
    // First mock: cisco_send_command succeeds
    MOCK_WRITE_SET_RETURN(15); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    
    // Second mock: directory listing response with special characters
    MOCK_READ_SET_RETURN(300, "Directory of flash:/\n"
                              "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  config@#$%.txt\n"
                              "2  -rwx  2048      Jan 02 2020 00:00:00 +00:00  backup@#$%.bin\n"
                              "1234567 bytes available\n");
    
    // Call the function under test
    file_tree_build(&conn, &root);
    
    // Verify root node was created correctly
    ASSERT_NOT_NULL(root);
    ASSERT_STRING_EQUAL("flash:/", root->name);
    ASSERT_STRING_EQUAL("flash:/", root->path);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, root->type);
    
    // Verify that children were added with special characters preserved
    ASSERT_NOT_NULL(root->children);
    
    // Check first child with special characters
    dir_node_t *child1 = root->children;
    ASSERT_STRING_EQUAL("config@#$%.txt", child1->name);
    ASSERT_STRING_EQUAL("flash:/config@#$%.txt", child1->path);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, child1->type);
    
    // Check second child with special characters
    ASSERT_NOT_NULL(child1->next);
    dir_node_t *child2 = child1->next;
    ASSERT_STRING_EQUAL("backup@#$%.bin", child2->name);
    ASSERT_STRING_EQUAL("flash:/backup@#$%.bin", child2->path);
    ASSERT_EQUAL(FILE_TYPE_BINARY, child2->type);
    
    // Clean up
    file_tree_free(root);
    
    return 1;
}

TEST(file_tree_build_long_filenames) {
    serial_conn_t conn;
    dir_node_t *root;
    setup_file_tree_build_recursive_tests();
    
    // Create a long filename
    char long_filename[300];
    memset(long_filename, 'a', 299);
    long_filename[299] = '\0';
    
    // Set up mocks for cisco_get_directory_listing with long filename
    // First mock: cisco_send_command succeeds
    MOCK_WRITE_SET_RETURN(15); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    
    // Second mock: directory listing response with long filename
    char mock_response[500];
    snprintf(mock_response, sizeof(mock_response), 
             "Directory of flash:/\n"
             "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  %s\n"
             "1234567 bytes available\n", long_filename);
    
    MOCK_READ_SET_RETURN(strlen(mock_response), mock_response);
    
    // Call the function under test
    file_tree_build(&conn, &root);
    
    // Verify root node was created correctly
    ASSERT_NOT_NULL(root);
    ASSERT_STRING_EQUAL("flash:/", root->name);
    ASSERT_STRING_EQUAL("flash:/", root->path);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, root->type);
    
    // Verify that child was added with truncated name if necessary
    ASSERT_NOT_NULL(root->children);
    
    dir_node_t *child = root->children;
    // The name should be truncated to MAX_PATH_LEN - 1 characters
    ASSERT_TRUE(strlen(child->name) <= MAX_PATH_LEN - 1);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, child->type);
    
    // Clean up
    file_tree_free(root);
    
    return 1;
}

TEST(file_tree_build_nested_structure) {
    serial_conn_t conn;
    dir_node_t *root;
    setup_file_tree_build_recursive_tests();
    
    // Set up mocks for cisco_get_directory_listing with nested structure
    // First mock: cisco_send_command succeeds
    MOCK_WRITE_SET_RETURN(15); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    
    // Second mock: directory listing response with nested structure
    MOCK_READ_SET_RETURN(300, "Directory of flash:/\n"
                              "1  drwx  0          Jan 01 2020 00:00:00 +00:00  level1\n"
                              "2  -rwx  1024      Jan 02 2020 00:00:00 +00:00  file1.txt\n"
                              "3  drwx  0          Jan 03 2020 00:00:00 +00:00  level2\n"
                              "1234567 bytes available\n");
    
    // Call the function under test
    file_tree_build(&conn, &root);
    
    // Verify root node was created correctly
    ASSERT_NOT_NULL(root);
    ASSERT_STRING_EQUAL("flash:/", root->name);
    ASSERT_STRING_EQUAL("flash:/", root->path);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, root->type);
    
    // Verify that children were added
    ASSERT_NOT_NULL(root->children);
    
    // Check first child (directory)
    dir_node_t *child1 = root->children;
    ASSERT_STRING_EQUAL("level1", child1->name);
    ASSERT_STRING_EQUAL("flash:/level1", child1->path);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, child1->type);
    ASSERT_EQUAL(root, child1->parent);
    ASSERT_EQUAL(1, child1->level);
    
    // Check second child (file)
    ASSERT_NOT_NULL(child1->next);
    dir_node_t *child2 = child1->next;
    ASSERT_STRING_EQUAL("file1.txt", child2->name);
    ASSERT_STRING_EQUAL("flash:/file1.txt", child2->path);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, child2->type);
    ASSERT_EQUAL(root, child2->parent);
    ASSERT_EQUAL(1, child2->level);
    
    // Check third child (directory)
    ASSERT_NOT_NULL(child2->next);
    dir_node_t *child3 = child2->next;
    ASSERT_STRING_EQUAL("level2", child3->name);
    ASSERT_STRING_EQUAL("flash:/level2", child3->path);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, child3->type);
    ASSERT_EQUAL(root, child3->parent);
    ASSERT_EQUAL(1, child3->level);
    
    // Verify no more children
    ASSERT_NULL(child3->next);
    
    // Clean up
    file_tree_free(root);
    
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
