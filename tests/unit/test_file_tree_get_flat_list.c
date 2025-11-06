/**
 * @file test_file_tree_get_flat_list.c
 * @brief Unit tests for file_tree_get_flat_list function
 *
 * This file contains unit tests for the file_tree_get_flat_list function.
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
TEST(file_tree_get_flat_list_null_node);
TEST(file_tree_get_flat_list_null_list);
TEST(file_tree_get_flat_list_zero_max_count);
TEST(file_tree_get_flat_list_single_node);
TEST(file_tree_get_flat_list_single_node_expanded);
TEST(file_tree_get_flat_list_single_node_collapsed);
TEST(file_tree_get_flat_list_multiple_children_expanded);
TEST(file_tree_get_flat_list_multiple_children_collapsed);
TEST(file_tree_get_flat_list_nested_structure_expanded);
TEST(file_tree_get_flat_list_nested_structure_partially_expanded);
TEST(file_tree_get_flat_list_max_count_limit);
TEST(file_tree_get_flat_list_max_count_exceeded);
TEST(file_tree_get_flat_list_complex_tree_all_expanded);
TEST(file_tree_get_flat_list_complex_tree_mixed_expansion);
TEST(file_tree_get_flat_list_empty_tree);
TEST(file_tree_get_flat_list_negative_max_count);
TEST(file_tree_get_flat_list_large_tree);
TEST(file_tree_get_flat_list_mixed_file_types);
TEST(file_tree_get_flat_list_deep_nesting);

// Test registry - all test functions
test_func_t test_registry[] = {
    test_file_tree_get_flat_list_null_node,
    test_file_tree_get_flat_list_null_list,
    test_file_tree_get_flat_list_zero_max_count,
    test_file_tree_get_flat_list_single_node,
    test_file_tree_get_flat_list_single_node_expanded,
    test_file_tree_get_flat_list_single_node_collapsed,
    test_file_tree_get_flat_list_multiple_children_expanded,
    test_file_tree_get_flat_list_multiple_children_collapsed,
    test_file_tree_get_flat_list_nested_structure_expanded,
    test_file_tree_get_flat_list_nested_structure_partially_expanded,
    test_file_tree_get_flat_list_max_count_limit,
    test_file_tree_get_flat_list_max_count_exceeded,
    test_file_tree_get_flat_list_complex_tree_all_expanded,
    test_file_tree_get_flat_list_complex_tree_mixed_expansion,
    test_file_tree_get_flat_list_empty_tree,
    test_file_tree_get_flat_list_negative_max_count,
    test_file_tree_get_flat_list_large_tree,
    test_file_tree_get_flat_list_mixed_file_types,
    test_file_tree_get_flat_list_deep_nesting,
    NULL};

// Test names for display
const char *test_names[] = {
    "file_tree_get_flat_list_null_node",
    "file_tree_get_flat_list_null_list",
    "file_tree_get_flat_list_zero_max_count",
    "file_tree_get_flat_list_single_node",
    "file_tree_get_flat_list_single_node_expanded",
    "file_tree_get_flat_list_single_node_collapsed",
    "file_tree_get_flat_list_multiple_children_expanded",
    "file_tree_get_flat_list_multiple_children_collapsed",
    "file_tree_get_flat_list_nested_structure_expanded",
    "file_tree_get_flat_list_nested_structure_partially_expanded",
    "file_tree_get_flat_list_max_count_limit",
    "file_tree_get_flat_list_max_count_exceeded",
    "file_tree_get_flat_list_complex_tree_all_expanded",
    "file_tree_get_flat_list_complex_tree_mixed_expansion",
    "file_tree_get_flat_list_empty_tree",
    "file_tree_get_flat_list_negative_max_count",
    "file_tree_get_flat_list_large_tree",
    "file_tree_get_flat_list_mixed_file_types",
    "file_tree_get_flat_list_deep_nesting",
    NULL};

// Test counts
int test_registry_count = sizeof(test_registry) / sizeof(test_registry[0]) - 1;
int test_names_count = sizeof(test_names) / sizeof(test_names[0]) - 1;

// ============================================================================
// Setup Functions
// ============================================================================

/**
 * @brief Setup function for file_tree_get_flat_list tests
 */
static void setup_file_tree_get_flat_list_tests(void) {
  // No specific setup needed for file_tree_get_flat_list tests
  // as it doesn't use mocks or external dependencies
}

// ============================================================================
// file_tree_get_flat_list Tests
// ============================================================================

TEST(file_tree_get_flat_list_null_node) {
  setup_file_tree_get_flat_list_tests();
  dir_node_t *list[10];
  int count = 0;

  // Test with NULL node
  int result = file_tree_get_flat_list(NULL, list, 10, &count);

  ASSERT_EQUAL(FILE_TREE_ERROR_NULL_PARAM, result);

  return 1;
}

TEST(file_tree_get_flat_list_null_list) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

  dir_node_t *node = NULL;

  int result =
      file_tree_create("test", "flash:/test", FILE_TYPE_REGULAR, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);

  ASSERT_NOT_NULL(node);

  // Test with NULL list
  result = file_tree_get_flat_list(node, NULL, 10, &count);
  ASSERT_EQUAL(FILE_TREE_ERROR_NULL_PARAM, result);

  // Clean up
  free(node);

  return 1;
}

TEST(file_tree_get_flat_list_zero_max_count) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

  dir_node_t *node = NULL;

  int result =
      file_tree_create("test", "flash:/test", FILE_TYPE_REGULAR, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);
  dir_node_t *list[10];

  ASSERT_NOT_NULL(node);

  // Test with zero max_count
  result = file_tree_get_flat_list(node, list, 0, &count);
  ASSERT_EQUAL(FILE_TREE_ERROR_NULL_PARAM, result);

  // Clean up
  free(node);

  return 1;
}

TEST(file_tree_get_flat_list_single_node) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

  dir_node_t *node = NULL;

  int result =
      file_tree_create("test", "flash:/test", FILE_TYPE_REGULAR, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);
  dir_node_t *list[10];

  ASSERT_NOT_NULL(node);

  // Test with single node
  result = file_tree_get_flat_list(node, list, 10, &count);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_EQUAL(1, count);
  ASSERT_EQUAL(node, list[0]);

  // Clean up
  free(node);

  return 1;
}

TEST(file_tree_get_flat_list_single_node_expanded) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

  dir_node_t *parent = NULL;

  int result =
      file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY, &parent);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(parent);
  dir_node_t *child = NULL;

  result = file_tree_create("child", "flash:/parent/child", FILE_TYPE_REGULAR,
                            &child);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child);
  dir_node_t *list[10];

  ASSERT_NOT_NULL(parent);
  ASSERT_NOT_NULL(child);

  // Add child to parent
  result = file_tree_add_child(parent, child);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Set parent as expanded
  parent->expanded = 1;

  // Test with expanded parent
  result = file_tree_get_flat_list(parent, list, 10, &count);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_EQUAL(2, count);
  ASSERT_EQUAL(parent, list[0]);
  ASSERT_EQUAL(child, list[1]);

  // Clean up
  file_tree_free(parent);

  return 1;
}

TEST(file_tree_get_flat_list_single_node_collapsed) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

  dir_node_t *parent = NULL;

  int result =
      file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY, &parent);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(parent);
  dir_node_t *child = NULL;

  result = file_tree_create("child", "flash:/parent/child", FILE_TYPE_REGULAR,
                            &child);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child);
  dir_node_t *list[10];

  ASSERT_NOT_NULL(parent);
  ASSERT_NOT_NULL(child);

  // Add child to parent
  result = file_tree_add_child(parent, child);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Set parent as collapsed (default state)
  parent->expanded = 0;

  // Test with collapsed parent
  result = file_tree_get_flat_list(parent, list, 10, &count);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_EQUAL(1, count);
  ASSERT_EQUAL(parent, list[0]);
  // Child should not be included

  // Clean up
  file_tree_free(parent);

  return 1;
}

TEST(file_tree_get_flat_list_multiple_children_expanded) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

  dir_node_t *parent = NULL;

  int result =
      file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY, &parent);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(parent);
  dir_node_t *child1 = NULL;

  result = file_tree_create("child1", "flash:/parent/child1", FILE_TYPE_REGULAR,
                            &child1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child1);
  dir_node_t *child2 = NULL;

  result = file_tree_create("child2", "flash:/parent/child2", FILE_TYPE_REGULAR,
                            &child2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child2);
  dir_node_t *child3 = NULL;

  result = file_tree_create("child3", "flash:/parent/child3", FILE_TYPE_REGULAR,
                            &child3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child3);
  dir_node_t *list[10];

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

  // Set parent as expanded
  parent->expanded = 1;

  // Test with expanded parent and multiple children
  result = file_tree_get_flat_list(parent, list, 10, &count);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_EQUAL(4, count);
  ASSERT_EQUAL(parent, list[0]);
  ASSERT_EQUAL(child1, list[1]);
  ASSERT_EQUAL(child2, list[2]);
  ASSERT_EQUAL(child3, list[3]);

  // Clean up
  file_tree_free(parent);

  return 1;
}

TEST(file_tree_get_flat_list_multiple_children_collapsed) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

  dir_node_t *parent = NULL;

  int result =
      file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY, &parent);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(parent);
  dir_node_t *child1 = NULL;

  result = file_tree_create("child1", "flash:/parent/child1", FILE_TYPE_REGULAR,
                            &child1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child1);
  dir_node_t *child2 = NULL;

  result = file_tree_create("child2", "flash:/parent/child2", FILE_TYPE_REGULAR,
                            &child2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child2);
  dir_node_t *child3 = NULL;

  result = file_tree_create("child3", "flash:/parent/child3", FILE_TYPE_REGULAR,
                            &child3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child3);
  dir_node_t *list[10];

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

  // Set parent as collapsed (default state)
  parent->expanded = 0;

  // Test with collapsed parent and multiple children
  result = file_tree_get_flat_list(parent, list, 10, &count);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_EQUAL(1, count);
  ASSERT_EQUAL(parent, list[0]);
  // Children should not be included

  // Clean up
  file_tree_free(parent);

  return 1;
}

TEST(file_tree_get_flat_list_nested_structure_expanded) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

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

  result =
      file_tree_create("file1", "flash:/root/file1", FILE_TYPE_REGULAR, &file1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file1);
  dir_node_t *file2 = NULL;

  result = file_tree_create("file2", "flash:/root/dir1/file2",
                            FILE_TYPE_REGULAR, &file2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file2);
  dir_node_t *file3 = NULL;

  result = file_tree_create("file3", "flash:/root/dir2/file3",
                            FILE_TYPE_REGULAR, &file3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file3);
  dir_node_t *list[10];

  ASSERT_NOT_NULL(root);
  ASSERT_NOT_NULL(dir1);
  ASSERT_NOT_NULL(dir2);
  ASSERT_NOT_NULL(file1);
  ASSERT_NOT_NULL(file2);
  ASSERT_NOT_NULL(file3);

  // Build nested structure
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

  // Set all directories as expanded
  root->expanded = 1;
  dir1->expanded = 1;
  dir2->expanded = 1;

  // Test with fully expanded nested structure
  result = file_tree_get_flat_list(root, list, 10, &count);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_EQUAL(6, count);
  ASSERT_EQUAL(root, list[0]);
  ASSERT_EQUAL(dir1, list[1]);
  ASSERT_EQUAL(file2, list[2]);
  ASSERT_EQUAL(dir2, list[3]);
  ASSERT_EQUAL(file3, list[4]);
  ASSERT_EQUAL(file1, list[5]);

  // Clean up
  file_tree_free(root);

  return 1;
}

TEST(file_tree_get_flat_list_nested_structure_partially_expanded) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

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

  result =
      file_tree_create("file1", "flash:/root/file1", FILE_TYPE_REGULAR, &file1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file1);
  dir_node_t *file2 = NULL;

  result = file_tree_create("file2", "flash:/root/dir1/file2",
                            FILE_TYPE_REGULAR, &file2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file2);
  dir_node_t *file3 = NULL;

  result = file_tree_create("file3", "flash:/root/dir2/file3",
                            FILE_TYPE_REGULAR, &file3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file3);
  dir_node_t *list[10];

  ASSERT_NOT_NULL(root);
  ASSERT_NOT_NULL(dir1);
  ASSERT_NOT_NULL(dir2);
  ASSERT_NOT_NULL(file1);
  ASSERT_NOT_NULL(file2);
  ASSERT_NOT_NULL(file3);

  // Build nested structure
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

  // Set only root as expanded, dir1 and dir2 collapsed
  root->expanded = 1;
  dir1->expanded = 0;
  dir2->expanded = 0;

  // Test with partially expanded nested structure
  result = file_tree_get_flat_list(root, list, 10, &count);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_EQUAL(4, count);
  ASSERT_EQUAL(root, list[0]);
  ASSERT_EQUAL(dir1, list[1]);
  ASSERT_EQUAL(dir2, list[2]);
  ASSERT_EQUAL(file1, list[3]);
  // file2 and file3 should not be included because their parents are collapsed

  // Clean up
  file_tree_free(root);

  return 1;
}

TEST(file_tree_get_flat_list_max_count_limit) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

  dir_node_t *parent = NULL;

  int result =
      file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY, &parent);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(parent);
  dir_node_t *child1 = NULL;

  result = file_tree_create("child1", "flash:/parent/child1", FILE_TYPE_REGULAR,
                            &child1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child1);
  dir_node_t *child2 = NULL;

  result = file_tree_create("child2", "flash:/parent/child2", FILE_TYPE_REGULAR,
                            &child2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child2);
  dir_node_t *child3 = NULL;

  result = file_tree_create("child3", "flash:/parent/child3", FILE_TYPE_REGULAR,
                            &child3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child3);
  dir_node_t *list[3]; // Only space for 3 items

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

  // Set parent as expanded
  parent->expanded = 1;

  // Test with max_count limit (3 items: parent + 2 children)
  result = file_tree_get_flat_list(parent, list, 3, &count);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_EQUAL(3, count);
  ASSERT_EQUAL(parent, list[0]);
  ASSERT_EQUAL(child1, list[1]);
  ASSERT_EQUAL(child2, list[2]);
  // child3 should not be included due to max_count limit

  // Clean up
  file_tree_free(parent);

  return 1;
}

TEST(file_tree_get_flat_list_max_count_exceeded) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

  dir_node_t *parent = NULL;

  int result =
      file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY, &parent);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(parent);
  dir_node_t *child1 = NULL;

  result = file_tree_create("child1", "flash:/parent/child1", FILE_TYPE_REGULAR,
                            &child1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child1);
  dir_node_t *child2 = NULL;

  result = file_tree_create("child2", "flash:/parent/child2", FILE_TYPE_REGULAR,
                            &child2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child2);
  dir_node_t *child3 = NULL;

  result = file_tree_create("child3", "flash:/parent/child3", FILE_TYPE_REGULAR,
                            &child3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child3);
  dir_node_t *list[2]; // Only space for 2 items

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

  // Set parent as expanded
  parent->expanded = 1;

  // Test with max_count exceeded (only 2 items: parent + 1 child)
  result = file_tree_get_flat_list(parent, list, 2, &count);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_EQUAL(2, count);
  ASSERT_EQUAL(parent, list[0]);
  ASSERT_EQUAL(child1, list[1]);
  // child2 and child3 should not be included due to max_count limit

  // Clean up
  file_tree_free(parent);

  return 1;
}

TEST(file_tree_get_flat_list_complex_tree_all_expanded) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

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
  dir_node_t *subdir1 = NULL;

  result = file_tree_create("subdir1", "flash:/root/dir1/subdir1",
                            FILE_TYPE_DIRECTORY, &subdir1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(subdir1);
  dir_node_t *file1 = NULL;

  result =
      file_tree_create("file1", "flash:/root/file1", FILE_TYPE_REGULAR, &file1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file1);
  dir_node_t *file2 = NULL;

  result = file_tree_create("file2", "flash:/root/dir1/file2",
                            FILE_TYPE_REGULAR, &file2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file2);
  dir_node_t *file3 = NULL;

  result = file_tree_create("file3", "flash:/root/dir2/file3",
                            FILE_TYPE_REGULAR, &file3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file3);
  dir_node_t *file4 = NULL;

  result = file_tree_create("file4", "flash:/root/dir1/subdir1/file4",
                            FILE_TYPE_REGULAR, &file4);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file4);
  dir_node_t *list[10];

  ASSERT_NOT_NULL(root);
  ASSERT_NOT_NULL(dir1);
  ASSERT_NOT_NULL(dir2);
  ASSERT_NOT_NULL(subdir1);
  ASSERT_NOT_NULL(file1);
  ASSERT_NOT_NULL(file2);
  ASSERT_NOT_NULL(file3);
  ASSERT_NOT_NULL(file4);

  // Build complex tree structure
  result = file_tree_add_child(root, dir1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(root, dir2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(root, file1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(dir1, file2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(dir1, subdir1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(dir2, file3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(subdir1, file4);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Set all directories as expanded
  root->expanded = 1;
  dir1->expanded = 1;
  dir2->expanded = 1;
  subdir1->expanded = 1;

  // Test with fully expanded complex tree
  result = file_tree_get_flat_list(root, list, 10, &count);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_EQUAL(8, count);
  ASSERT_EQUAL(root, list[0]);
  ASSERT_EQUAL(dir1, list[1]);
  ASSERT_EQUAL(file2, list[2]);
  ASSERT_EQUAL(subdir1, list[3]);
  ASSERT_EQUAL(file4, list[4]);
  ASSERT_EQUAL(dir2, list[5]);
  ASSERT_EQUAL(file3, list[6]);
  ASSERT_EQUAL(file1, list[7]);

  // Clean up
  file_tree_free(root);

  return 1;
}

TEST(file_tree_get_flat_list_complex_tree_mixed_expansion) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

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
  dir_node_t *subdir1 = NULL;

  result = file_tree_create("subdir1", "flash:/root/dir1/subdir1",
                            FILE_TYPE_DIRECTORY, &subdir1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(subdir1);
  dir_node_t *file1 = NULL;

  result =
      file_tree_create("file1", "flash:/root/file1", FILE_TYPE_REGULAR, &file1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file1);
  dir_node_t *file2 = NULL;

  result = file_tree_create("file2", "flash:/root/dir1/file2",
                            FILE_TYPE_REGULAR, &file2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file2);
  dir_node_t *file3 = NULL;

  result = file_tree_create("file3", "flash:/root/dir2/file3",
                            FILE_TYPE_REGULAR, &file3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file3);
  dir_node_t *file4 = NULL;

  result = file_tree_create("file4", "flash:/root/dir1/subdir1/file4",
                            FILE_TYPE_REGULAR, &file4);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file4);
  dir_node_t *list[10];

  ASSERT_NOT_NULL(root);
  ASSERT_NOT_NULL(dir1);
  ASSERT_NOT_NULL(dir2);
  ASSERT_NOT_NULL(subdir1);
  ASSERT_NOT_NULL(file1);
  ASSERT_NOT_NULL(file2);
  ASSERT_NOT_NULL(file3);
  ASSERT_NOT_NULL(file4);

  // Build complex tree structure
  result = file_tree_add_child(root, dir1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(root, dir2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(root, file1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(dir1, file2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(dir1, subdir1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(dir2, file3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(subdir1, file4);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Set mixed expansion: root expanded, dir1 expanded, dir2 collapsed, subdir1
  // collapsed
  root->expanded = 1;
  dir1->expanded = 1;
  dir2->expanded = 0;
  subdir1->expanded = 0;

  // Test with mixed expansion
  result = file_tree_get_flat_list(root, list, 10, &count);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_EQUAL(6, count);
  ASSERT_EQUAL(root, list[0]);
  ASSERT_EQUAL(dir1, list[1]);
  ASSERT_EQUAL(file2, list[2]);
  ASSERT_EQUAL(subdir1, list[3]);
  ASSERT_EQUAL(dir2, list[4]);
  ASSERT_EQUAL(file1, list[5]);
  // file3 should not be included because dir2 is collapsed
  // file4 should not be included because subdir1 is collapsed

  // Clean up
  file_tree_free(root);

  return 1;
}

TEST(file_tree_get_flat_list_empty_tree) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

  dir_node_t *node = NULL;

  int result =
      file_tree_create("empty", "flash:/empty", FILE_TYPE_DIRECTORY, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);
  dir_node_t *list[10];

  ASSERT_NOT_NULL(node);

  // Verify it has no children
  ASSERT_NULL(node->children);

  // Test with empty tree (no children)
  result = file_tree_get_flat_list(node, list, 10, &count);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_EQUAL(1, count);
  ASSERT_EQUAL(node, list[0]);

  // Clean up
  free(node);

  return 1;
}

TEST(file_tree_get_flat_list_negative_max_count) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

  dir_node_t *node = NULL;

  int result =
      file_tree_create("test", "flash:/test", FILE_TYPE_REGULAR, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);
  dir_node_t *list[10];

  ASSERT_NOT_NULL(node);

  // Test with negative max_count
  result = file_tree_get_flat_list(node, list, -1, &count);
  ASSERT_EQUAL(FILE_TREE_ERROR_NULL_PARAM, result);

  // Clean up
  free(node);

  return 1;
}

TEST(file_tree_get_flat_list_large_tree) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

  dir_node_t *root = NULL;

  int result =
      file_tree_create("root", "flash:/root", FILE_TYPE_DIRECTORY, &root);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(root);
  dir_node_t *list[20];

  ASSERT_NOT_NULL(root);

  // Create a large tree with many children
  for (int i = 0; i < 15; i++) {
    char name[32], path[64];
    snprintf(name, sizeof(name), "file%d", i);
    snprintf(path, sizeof(path), "flash:/root/file%d", i);

    dir_node_t *child = NULL;

    result = file_tree_create(name, path, FILE_TYPE_REGULAR, &child);

    ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

    ASSERT_NOT_NULL(child);
    ASSERT_NOT_NULL(child);
    result = file_tree_add_child(root, child);

    ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  }

  // Set root as expanded
  root->expanded = 1;

  // Test with large tree
  result = file_tree_get_flat_list(root, list, 20, &count);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_EQUAL(16, count); // root + 15 children
  ASSERT_EQUAL(root, list[0]);

  // Verify all children are included
  for (int i = 1; i < 16; i++) {
    ASSERT_NOT_NULL(list[i]);
  }

  // Clean up
  file_tree_free(root);

  return 1;
}

TEST(file_tree_get_flat_list_mixed_file_types) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

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
  dir_node_t *file1 = NULL;

  result = file_tree_create("file1.txt", "flash:/root/file1.txt",
                            FILE_TYPE_REGULAR, &file1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file1);
  dir_node_t *file2 = NULL;

  result = file_tree_create("file2.bin", "flash:/root/file2.bin",
                            FILE_TYPE_BINARY, &file2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file2);
  dir_node_t *file3 = NULL;

  result = file_tree_create("file3.txt", "flash:/root/dir1/file3.txt",
                            FILE_TYPE_REGULAR, &file3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(file3);
  dir_node_t *list[10];

  ASSERT_NOT_NULL(root);
  ASSERT_NOT_NULL(dir1);
  ASSERT_NOT_NULL(file1);
  ASSERT_NOT_NULL(file2);
  ASSERT_NOT_NULL(file3);

  // Build tree with mixed file types
  result = file_tree_add_child(root, dir1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(root, file1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(root, file2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(dir1, file3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Set root as expanded
  root->expanded = 1;

  // Test with mixed file types
  result = file_tree_get_flat_list(root, list, 10, &count);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_EQUAL(4, count);
  ASSERT_EQUAL(root, list[0]);
  ASSERT_EQUAL(dir1, list[1]);
  ASSERT_EQUAL(file1, list[2]);
  ASSERT_EQUAL(file2, list[3]);
  // file3 should not be included because dir1 is collapsed

  // Verify file types are preserved
  ASSERT_EQUAL(FILE_TYPE_DIRECTORY, list[0]->type);
  ASSERT_EQUAL(FILE_TYPE_DIRECTORY, list[1]->type);
  ASSERT_EQUAL(FILE_TYPE_REGULAR, list[2]->type);
  ASSERT_EQUAL(FILE_TYPE_BINARY, list[3]->type);

  // Clean up
  file_tree_free(root);

  return 1;
}

TEST(file_tree_get_flat_list_deep_nesting) {
  setup_file_tree_get_flat_list_tests();
  int count = 0;

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
  dir_node_t *list[10];

  ASSERT_NOT_NULL(level0);
  ASSERT_NOT_NULL(level1);
  ASSERT_NOT_NULL(level2);
  ASSERT_NOT_NULL(level3);
  ASSERT_NOT_NULL(file);

  // Build deep nested structure
  result = file_tree_add_child(level0, level1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(level1, level2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(level2, level3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(level3, file);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Set all directories as expanded
  level0->expanded = 1;
  level1->expanded = 1;
  level2->expanded = 1;
  level3->expanded = 1;

  // Test with deep nesting
  result = file_tree_get_flat_list(level0, list, 10, &count);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_EQUAL(5, count);
  ASSERT_EQUAL(level0, list[0]);
  ASSERT_EQUAL(level1, list[1]);
  ASSERT_EQUAL(level2, list[2]);
  ASSERT_EQUAL(level3, list[3]);
  ASSERT_EQUAL(file, list[4]);

  // Clean up
  file_tree_free(level0);

  return 1;
}

int main(void) {
  printf("Running file_tree unit tests...\n");

  int result = run_all_tests();

  return result ? 0 : 1;
}
