/**
 * @file test_file_tree_count_selected.c
 * @brief Unit tests for file_tree_count_selected function
 *
 * This file contains unit tests for the file_tree_count_selected function.
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

// Test registry - all test functions
test_func_t test_registry[] = {
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
    NULL};

// Test names for display
const char *test_names[] = {
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
    NULL};

// Test counts
int test_registry_count = sizeof(test_registry) / sizeof(test_registry[0]) - 1;
int test_names_count = sizeof(test_names) / sizeof(test_names[0]) - 1;

// ============================================================================
// Setup Functions
// ============================================================================

/**
 * @brief Setup function for file_tree_count_selected tests
 */
static void setup_file_tree_count_selected_tests(void) {
  // No specific setup needed for file_tree_count_selected tests
  // as it doesn't use mocks or external dependencies
}

// ============================================================================
// file_tree_count_selected Tests
// ============================================================================

TEST(file_tree_count_selected_null_node) {
  setup_file_tree_count_selected_tests();

  // Test counting selected nodes with NULL node and NULL count
  int result = file_tree_count_selected(NULL, NULL);
  ASSERT_EQUAL(FILE_TREE_ERROR_NULL_PARAM, result);

  // Test counting selected nodes with NULL node but valid count
  int count = 0;
  result = file_tree_count_selected(NULL, &count);
  ASSERT_EQUAL(FILE_TREE_ERROR_NULL_PARAM, result);

  // Test counting selected nodes with valid node but NULL count
  dir_node_t *node = NULL;
  result = file_tree_create("test", "flash:/test", FILE_TYPE_REGULAR, &node);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  ASSERT_NOT_NULL(node);

  result = file_tree_count_selected(node, NULL);
  ASSERT_EQUAL(FILE_TREE_ERROR_NULL_PARAM, result);

  // Clean up
  file_tree_free(node);

  return 1;
}

TEST(file_tree_count_selected_single_unselected) {
  setup_file_tree_count_selected_tests();

  // Create a single unselected node
  dir_node_t *node = NULL;

  int result =
      file_tree_create("test.txt", "flash:/test.txt", FILE_TYPE_REGULAR, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);
  ASSERT_NOT_NULL(node);

  // Verify initial state
  ASSERT_EQUAL(0, node->selected);

  // Count selected nodes
  int count = 0;

  result = file_tree_count_selected(node, &count);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Should return 0 for unselected node
  ASSERT_EQUAL(0, count);

  // Clean up
  free(node);

  return 1;
}

TEST(file_tree_count_selected_single_selected) {
  setup_file_tree_count_selected_tests();

  // Create a single selected node
  dir_node_t *node = NULL;

  int result =
      file_tree_create("test.txt", "flash:/test.txt", FILE_TYPE_REGULAR, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);
  ASSERT_NOT_NULL(node);

  // Select the node
  node->selected = 1;

  // Count selected nodes
  int count = 0;

  result = file_tree_count_selected(node, &count);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Should return 1 for selected node
  ASSERT_EQUAL(1, count);

  // Clean up
  free(node);

  return 1;
}

TEST(file_tree_count_selected_multiple_children_none_selected) {
  setup_file_tree_count_selected_tests();

  // Create a parent with multiple children, none selected
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
  dir_node_t *child3 = NULL;

  result = file_tree_create("child3.txt", "flash:/parent/child3.txt",
                            FILE_TYPE_REGULAR, &child3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child3);

  ASSERT_NOT_NULL(parent);
  ASSERT_NOT_NULL(child1);
  ASSERT_NOT_NULL(child2);
  ASSERT_NOT_NULL(child3);

  // Add children to parent
  result = file_tree_add_child(parent, child1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(parent, child2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(parent, child3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Verify all nodes are unselected
  ASSERT_EQUAL(0, parent->selected);
  ASSERT_EQUAL(0, child1->selected);
  ASSERT_EQUAL(0, child2->selected);
  ASSERT_EQUAL(0, child3->selected);

  // Count selected nodes
  int count = 0;

  result = file_tree_count_selected(parent, &count);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Should return 0 for no selected nodes
  ASSERT_EQUAL(0, count);

  // Clean up
  file_tree_free(parent);

  return 1;
}

TEST(file_tree_count_selected_multiple_children_some_selected) {
  setup_file_tree_count_selected_tests();

  // Create a parent with multiple children, some selected
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
  dir_node_t *child3 = NULL;

  result = file_tree_create("child3.txt", "flash:/parent/child3.txt",
                            FILE_TYPE_REGULAR, &child3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child3);

  ASSERT_NOT_NULL(parent);
  ASSERT_NOT_NULL(child1);
  ASSERT_NOT_NULL(child2);
  ASSERT_NOT_NULL(child3);

  // Add children to parent
  result = file_tree_add_child(parent, child1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(parent, child2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(parent, child3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Select some children
  child1->selected = 1;
  child3->selected = 1;

  // Verify selection states
  ASSERT_EQUAL(0, parent->selected);
  ASSERT_EQUAL(1, child1->selected);
  ASSERT_EQUAL(0, child2->selected);
  ASSERT_EQUAL(1, child3->selected);

  // Count selected nodes
  int count = 0;

  result = file_tree_count_selected(parent, &count);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Should return 2 for 2 selected children
  ASSERT_EQUAL(2, count);

  // Clean up
  file_tree_free(parent);

  return 1;
}

TEST(file_tree_count_selected_multiple_children_all_selected) {
  setup_file_tree_count_selected_tests();

  // Create a parent with multiple children, all selected
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
  dir_node_t *child3 = NULL;

  result = file_tree_create("child3.txt", "flash:/parent/child3.txt",
                            FILE_TYPE_REGULAR, &child3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child3);

  ASSERT_NOT_NULL(parent);
  ASSERT_NOT_NULL(child1);
  ASSERT_NOT_NULL(child2);
  ASSERT_NOT_NULL(child3);

  // Add children to parent
  result = file_tree_add_child(parent, child1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(parent, child2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(parent, child3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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
  int count = 0;

  result = file_tree_count_selected(parent, &count);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Should return 3 for all 3 selected children
  ASSERT_EQUAL(3, count);

  // Clean up
  file_tree_free(parent);

  return 1;
}

TEST(file_tree_count_selected_complex_tree_partial_selection) {
  setup_file_tree_count_selected_tests();

  // Create a complex tree structure with partial selection
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

  // Select some nodes partially
  file1->selected = 1; // root level file
  file2->selected = 1; // file in dir1
  file4->selected = 1; // file in subdir

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
  int count = 0;

  result = file_tree_count_selected(root, &count);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Should return 3 for 3 selected files
  ASSERT_EQUAL(3, count);

  // Clean up
  file_tree_free(root);

  return 1;
}

TEST(file_tree_count_selected_complex_tree_full_selection) {
  setup_file_tree_count_selected_tests();

  // Create a complex tree structure with full selection
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
  int count = 0;

  result = file_tree_count_selected(root, &count);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Should return 8 for all 8 nodes selected
  ASSERT_EQUAL(8, count);

  // Clean up
  file_tree_free(root);

  return 1;
}

TEST(file_tree_count_selected_nested_structure) {
  setup_file_tree_count_selected_tests();

  // Create a deeply nested structure
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
  int count = 0;

  result = file_tree_count_selected(level0, &count);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Should return 3 for 3 selected nodes
  ASSERT_EQUAL(3, count);

  // Clean up
  file_tree_free(level0);

  return 1;
}

TEST(file_tree_count_selected_mixed_types) {
  setup_file_tree_count_selected_tests();

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

  // Build the mixed type tree
  result = file_tree_add_child(root, regular_file);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(root, binary_file);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(root, subdir);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(subdir, subfile);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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
  int count = 0;

  result = file_tree_count_selected(root, &count);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Should return 3 for 3 selected nodes (mixed types)
  ASSERT_EQUAL(3, count);

  // Clean up
  file_tree_free(root);

  return 1;
}

TEST(file_tree_count_selected_empty_tree) {
  setup_file_tree_count_selected_tests();

  // Create a node with no children (empty tree)
  dir_node_t *node = NULL;

  int result =
      file_tree_create("empty", "flash:/empty", FILE_TYPE_DIRECTORY, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);
  ASSERT_NOT_NULL(node);

  // Verify it has no children
  ASSERT_NULL(node->children);

  // Count selected nodes (none selected)
  int count = 0;

  result = file_tree_count_selected(node, &count);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Should return 0 for no selected nodes
  ASSERT_EQUAL(0, count);

  // Select the node
  node->selected = 1;

  // Count selected nodes again
  result = file_tree_count_selected(node, &count);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Should return 1 for the selected node
  ASSERT_EQUAL(1, count);

  // Clean up
  free(node);

  return 1;
}

int main(void) {
  printf("Running file_tree unit tests...\n");

  int result = run_all_tests();

  return result ? 0 : 1;
}
