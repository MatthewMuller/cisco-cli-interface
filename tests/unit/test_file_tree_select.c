/**
 * @file test_file_tree_select.c
 * @brief Unit tests for file_tree_select function
 *
 * This file contains unit tests for the file_tree_select function:
 * - Single file and directory selection
 * - Directory with children selection
 * - Complex tree structures
 * - Edge cases with NULL nodes
 * - Already selected nodes
 * - Deselection functionality
 * - Mixed file types
 * - Nested directories
 * - Multiple children
 *
 * @author Cisco CLI Interface Team
 * @version 1.0
 */

#include "../../include/cisco_cli.h"
#include "../framework/test_framework.h"

// ============================================================================
// Test Registry
// ============================================================================

// Forward declarations of all test functions
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

// Test registry - all test functions
test_func_t test_registry[] = {test_file_tree_select_single_file,
                               test_file_tree_select_single_directory,
                               test_file_tree_select_directory_with_children,
                               test_file_tree_select_complex_tree,
                               test_file_tree_select_null_node,
                               test_file_tree_select_already_selected,
                               test_file_tree_select_deselect,
                               test_file_tree_select_mixed_types,
                               test_file_tree_select_nested_directories,
                               test_file_tree_select_multiple_children,
                               NULL};

// Test names for display
const char *test_names[] = {"file_tree_select_single_file",
                            "file_tree_select_single_directory",
                            "file_tree_select_directory_with_children",
                            "file_tree_select_complex_tree",
                            "file_tree_select_null_node",
                            "file_tree_select_already_selected",
                            "file_tree_select_deselect",
                            "file_tree_select_mixed_types",
                            "file_tree_select_nested_directories",
                            "file_tree_select_multiple_children",
                            NULL};

// Test counts
int test_registry_count = sizeof(test_registry) / sizeof(test_registry[0]) - 1;
int test_names_count = sizeof(test_names) / sizeof(test_names[0]) - 1;

// ============================================================================
// Setup Functions
// ============================================================================

/**
 * @brief Setup function for file_tree_select tests
 */
static void setup_file_tree_select_tests(void) {
  // No specific setup needed for file_tree_select tests
  // as it doesn't use mocks or external dependencies
}

// ============================================================================
// file_tree_select Tests
// ============================================================================

TEST(file_tree_select_single_file) {
  setup_file_tree_select_tests();

  // Create a single file node
  dir_node_t *file = NULL;

  int result = file_tree_create("config.txt", "flash:/config.txt",
                                FILE_TYPE_REGULAR, &file);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file);
  ASSERT_NOT_NULL(file);

  // Verify initial state
  ASSERT_EQUAL(0, file->selected);

  // Select the file
  result = file_tree_select(file, 1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Verify file is now selected
  ASSERT_EQUAL(1, file->selected);

  // Clean up
  free(file);

  return 1;
}

TEST(file_tree_select_single_directory) {
  setup_file_tree_select_tests();

  // Create a single directory node
  dir_node_t *dir = NULL;

  int result =
      file_tree_create("backup", "flash:/backup", FILE_TYPE_DIRECTORY, &dir);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(dir);
  ASSERT_NOT_NULL(dir);

  // Verify initial state
  ASSERT_EQUAL(0, dir->selected);

  // Select the directory
  result = file_tree_select(dir, 1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Verify directory is now selected
  ASSERT_EQUAL(1, dir->selected);

  // Clean up
  free(dir);

  return 1;
}

TEST(file_tree_select_directory_with_children) {
  setup_file_tree_select_tests();

  // Create a directory with children
  dir_node_t *parent = NULL;

  int result =
      file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY, &parent);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(parent);
  dir_node_t *child1 = NULL;

  result = file_tree_create("child1.txt", "flash:/parent/child1.txt",
                            FILE_TYPE_REGULAR, &child1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child1);
  dir_node_t *child2 = NULL;

  result = file_tree_create("child2.txt", "flash:/parent/child2.txt",
                            FILE_TYPE_REGULAR, &child2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child2);

  ASSERT_NOT_NULL(parent);
  ASSERT_NOT_NULL(child1);
  ASSERT_NOT_NULL(child2);

  // Add children to parent
  result = file_tree_add_child(parent, child1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(parent, child2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Verify initial states
  ASSERT_EQUAL(0, parent->selected);
  ASSERT_EQUAL(0, child1->selected);
  ASSERT_EQUAL(0, child2->selected);

  // Select the parent directory
  result = file_tree_select(parent, 1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Verify parent and all children are selected
  ASSERT_EQUAL(1, parent->selected);
  ASSERT_EQUAL(1, child1->selected);
  ASSERT_EQUAL(1, child2->selected);

  // Clean up
  file_tree_free(parent);

  return 1;
}

TEST(file_tree_select_complex_tree) {
  setup_file_tree_select_tests();

  // Create a complex tree structure
  dir_node_t *root = NULL;

  int result =
      file_tree_create("root", "flash:/root", FILE_TYPE_DIRECTORY, &root);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(root);
  dir_node_t *dir1 = NULL;

  result =
      file_tree_create("dir1", "flash:/root/dir1", FILE_TYPE_DIRECTORY, &dir1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(dir1);
  dir_node_t *dir2 = NULL;

  result =
      file_tree_create("dir2", "flash:/root/dir2", FILE_TYPE_DIRECTORY, &dir2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(dir2);
  dir_node_t *file1 = NULL;

  result = file_tree_create("file1.txt", "flash:/root/file1.txt",
                            FILE_TYPE_REGULAR, &file1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file1);
  dir_node_t *file2 = NULL;

  result = file_tree_create("file2.txt", "flash:/root/dir1/file2.txt",
                            FILE_TYPE_REGULAR, &file2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file2);
  dir_node_t *file3 = NULL;

  result = file_tree_create("file3.txt", "flash:/root/dir2/file3.txt",
                            FILE_TYPE_REGULAR, &file3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file3);
  dir_node_t *subdir = NULL;

  result = file_tree_create("subdir", "flash:/root/dir1/subdir",
                            FILE_TYPE_DIRECTORY, &subdir);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(subdir);
  dir_node_t *file4 = NULL;

  result = file_tree_create("file4.txt", "flash:/root/dir1/subdir/file4.txt",
                            FILE_TYPE_REGULAR, &file4);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file4);

  ASSERT_NOT_NULL(root);
  ASSERT_NOT_NULL(dir1);
  ASSERT_NOT_NULL(dir2);
  ASSERT_NOT_NULL(file1);
  ASSERT_NOT_NULL(file2);
  ASSERT_NOT_NULL(file3);
  ASSERT_NOT_NULL(subdir);
  ASSERT_NOT_NULL(file4);

  // Build the tree structure
  result = file_tree_add_child(root, dir1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(root, dir2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(root, file1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(dir1, file2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(dir2, file3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(dir1, subdir);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(subdir, file4);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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
  result = file_tree_select(root, 1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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
  setup_file_tree_select_tests();

  int result;

  // Test selecting a NULL node - this should return error
  result = file_tree_select(NULL, 1);

  ASSERT_EQUAL(FILE_TREE_ERROR_NULL_PARAM, result);

  // If we reach here without crashing, the test passes
  return 1;
}

TEST(file_tree_select_already_selected) {
  setup_file_tree_select_tests();

  // Create a file node
  dir_node_t *file = NULL;

  int result = file_tree_create("config.txt", "flash:/config.txt",
                                FILE_TYPE_REGULAR, &file);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file);
  ASSERT_NOT_NULL(file);

  // Select the file
  result = file_tree_select(file, 1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  ASSERT_EQUAL(1, file->selected);

  // Select it again
  result = file_tree_select(file, 1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Verify it's still selected
  ASSERT_EQUAL(1, file->selected);

  // Clean up
  free(file);

  return 1;
}

TEST(file_tree_select_deselect) {
  setup_file_tree_select_tests();

  // Create a directory with children
  dir_node_t *parent = NULL;

  int result =
      file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY, &parent);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(parent);
  dir_node_t *child1 = NULL;

  result = file_tree_create("child1.txt", "flash:/parent/child1.txt",
                            FILE_TYPE_REGULAR, &child1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child1);
  dir_node_t *child2 = NULL;

  result = file_tree_create("child2.txt", "flash:/parent/child2.txt",
                            FILE_TYPE_REGULAR, &child2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child2);

  ASSERT_NOT_NULL(parent);
  ASSERT_NOT_NULL(child1);
  ASSERT_NOT_NULL(child2);

  // Add children to parent
  result = file_tree_add_child(parent, child1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(parent, child2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Select the parent directory
  result = file_tree_select(parent, 1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  ASSERT_EQUAL(1, parent->selected);
  ASSERT_EQUAL(1, child1->selected);
  ASSERT_EQUAL(1, child2->selected);

  // Deselect the parent directory
  result = file_tree_select(parent, 0);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Verify parent and all children are deselected
  ASSERT_EQUAL(0, parent->selected);
  ASSERT_EQUAL(0, child1->selected);
  ASSERT_EQUAL(0, child2->selected);

  // Clean up
  file_tree_free(parent);

  return 1;
}

TEST(file_tree_select_mixed_types) {
  setup_file_tree_select_tests();

  // Create a tree with mixed file types
  dir_node_t *root = NULL;

  int result =
      file_tree_create("mixed", "flash:/mixed", FILE_TYPE_DIRECTORY, &root);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(root);
  dir_node_t *regular_file = NULL;

  result = file_tree_create("regular.txt", "flash:/mixed/regular.txt",
                            FILE_TYPE_REGULAR, &regular_file);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(regular_file);
  dir_node_t *binary_file = NULL;

  result = file_tree_create("binary.bin", "flash:/mixed/binary.bin",
                            FILE_TYPE_BINARY, &binary_file);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(binary_file);
  dir_node_t *subdir = NULL;

  result = file_tree_create("subdir", "flash:/mixed/subdir",
                            FILE_TYPE_DIRECTORY, &subdir);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(subdir);
  dir_node_t *subfile = NULL;

  result = file_tree_create("subfile.txt", "flash:/mixed/subdir/subfile.txt",
                            FILE_TYPE_REGULAR, &subfile);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(subfile);

  ASSERT_NOT_NULL(root);
  ASSERT_NOT_NULL(regular_file);
  ASSERT_NOT_NULL(binary_file);
  ASSERT_NOT_NULL(subdir);
  ASSERT_NOT_NULL(subfile);

  // Build the tree
  result = file_tree_add_child(root, regular_file);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(root, binary_file);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(root, subdir);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(subdir, subfile);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Verify initial states
  ASSERT_EQUAL(0, root->selected);
  ASSERT_EQUAL(0, regular_file->selected);
  ASSERT_EQUAL(0, binary_file->selected);
  ASSERT_EQUAL(0, subdir->selected);
  ASSERT_EQUAL(0, subfile->selected);

  // Select the root directory
  result = file_tree_select(root, 1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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
  setup_file_tree_select_tests();

  // Create a deeply nested directory structure
  dir_node_t *level0 = NULL;

  int result =
      file_tree_create("level0", "flash:/level0", FILE_TYPE_DIRECTORY, &level0);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(level0);
  dir_node_t *level1 = NULL;

  result = file_tree_create("level1", "flash:/level0/level1",
                            FILE_TYPE_DIRECTORY, &level1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(level1);
  dir_node_t *level2 = NULL;

  result = file_tree_create("level2", "flash:/level0/level1/level2",
                            FILE_TYPE_DIRECTORY, &level2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(level2);
  dir_node_t *level3 = NULL;

  result = file_tree_create("level3", "flash:/level0/level1/level2/level3",
                            FILE_TYPE_DIRECTORY, &level3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(level3);
  dir_node_t *file = NULL;

  result = file_tree_create("file.txt",
                            "flash:/level0/level1/level2/level3/file.txt",
                            FILE_TYPE_REGULAR, &file);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file);

  ASSERT_NOT_NULL(level0);
  ASSERT_NOT_NULL(level1);
  ASSERT_NOT_NULL(level2);
  ASSERT_NOT_NULL(level3);
  ASSERT_NOT_NULL(file);

  // Build the nested structure
  result = file_tree_add_child(level0, level1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(level1, level2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(level2, level3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(level3, file);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Verify initial states
  ASSERT_EQUAL(0, level0->selected);
  ASSERT_EQUAL(0, level1->selected);
  ASSERT_EQUAL(0, level2->selected);
  ASSERT_EQUAL(0, level3->selected);
  ASSERT_EQUAL(0, file->selected);

  // Select the top-level directory
  result = file_tree_select(level0, 1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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
  setup_file_tree_select_tests();

  // Create a directory with many children
  dir_node_t *parent = NULL;

  int result =
      file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY, &parent);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(parent);
  dir_node_t *children[10];

  ASSERT_NOT_NULL(parent);

  // Create 10 child files
  for (int i = 0; i < 10; i++) {
    char name[32];
    char path[64];
    snprintf(name, sizeof(name), "child%d.txt", i);
    snprintf(path, sizeof(path), "flash:/parent/child%d.txt", i);

    result = file_tree_create(name, path, FILE_TYPE_REGULAR, &children[i]);
    ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
    ASSERT_NOT_NULL(children[i]);

    // Add child to parent
    result = file_tree_add_child(parent, children[i]);

    ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

    // Verify initial state
    ASSERT_EQUAL(0, children[i]->selected);
  }

  // Verify parent initial state
  ASSERT_EQUAL(0, parent->selected);

  // Select the parent directory
  result = file_tree_select(parent, 1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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

int main(void) {
  printf("Running file_tree unit tests...\n");

  int result = run_all_tests();

  return result ? 0 : 1;
}
