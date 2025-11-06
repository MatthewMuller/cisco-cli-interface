/**
 * @file test_file_tree_free.c
 * @brief Unit tests for file_tree_free function
 *
 * This file contains unit tests for the file_tree_free function:
 * - Single node freeing
 * - Multiple nodes freeing
 * - Complex tree structures
 * - Edge cases with NULL nodes
 * - Nested structures
 * - Mixed file types
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
TEST(file_tree_free_single_node);
TEST(file_tree_free_multiple_nodes);
TEST(file_tree_free_complex_tree);
TEST(file_tree_free_null_node);
TEST(file_tree_free_empty_tree);
TEST(file_tree_free_nested_structure);
TEST(file_tree_free_mixed_types);

// Test registry - all test functions
test_func_t test_registry[] = {
    test_file_tree_free_single_node,  test_file_tree_free_multiple_nodes,
    test_file_tree_free_complex_tree, test_file_tree_free_null_node,
    test_file_tree_free_empty_tree,   test_file_tree_free_nested_structure,
    test_file_tree_free_mixed_types,  NULL};

// Test names for display
const char *test_names[] = {
    "file_tree_free_single_node",  "file_tree_free_multiple_nodes",
    "file_tree_free_complex_tree", "file_tree_free_null_node",
    "file_tree_free_empty_tree",   "file_tree_free_nested_structure",
    "file_tree_free_mixed_types",  NULL};

// Test counts
int test_registry_count = sizeof(test_registry) / sizeof(test_registry[0]) - 1;
int test_names_count = sizeof(test_names) / sizeof(test_names[0]) - 1;

// ============================================================================
// Setup Functions
// ============================================================================

/**
 * @brief Setup function for file_tree_free tests
 */
static void setup_file_tree_free_tests(void) {
  // No specific setup needed for file_tree_free tests
  // as it doesn't use mocks or external dependencies
}

// ============================================================================
// file_tree_free Tests
// ============================================================================

TEST(file_tree_free_single_node) {
  setup_file_tree_free_tests();

  // Create a single node
  dir_node_t *node = NULL;

  int result =
      file_tree_create("test.txt", "flash:/test.txt", FILE_TYPE_REGULAR, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);
  ASSERT_NOT_NULL(node);

  // Free the node - this should not cause any crashes
  result = file_tree_free(node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // If we reach here without crashing, the test passes
  return 1;
}

TEST(file_tree_free_multiple_nodes) {
  setup_file_tree_free_tests();

  // Create multiple nodes in a simple linked list structure
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

  // Verify structure is correct
  ASSERT_EQUAL(child1, parent->children);
  ASSERT_EQUAL(child2, child1->next);
  ASSERT_EQUAL(child3, child2->next);
  ASSERT_NULL(child3->next);

  // Free the entire tree - this should free all nodes recursively
  result = file_tree_free(parent);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // If we reach here without crashing, the test passes
  return 1;
}

TEST(file_tree_free_complex_tree) {
  setup_file_tree_free_tests();

  // Create a complex tree structure with multiple levels
  dir_node_t *root = NULL;

  int result = file_tree_create("root", "flash:/", FILE_TYPE_DIRECTORY, &root);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(root);
  dir_node_t *level1_a = NULL;

  result = file_tree_create("level1_a", "flash:/level1_a", FILE_TYPE_DIRECTORY,
                            &level1_a);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(level1_a);
  dir_node_t *level1_b = NULL;

  result = file_tree_create("level1_b", "flash:/level1_b", FILE_TYPE_DIRECTORY,
                            &level1_b);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(level1_b);
  dir_node_t *level2_a1 = NULL;

  result = file_tree_create("level2_a1", "flash:/level1_a/level2_a1",
                            FILE_TYPE_REGULAR, &level2_a1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(level2_a1);
  dir_node_t *level2_a2 = NULL;

  result = file_tree_create("level2_a2", "flash:/level1_a/level2_a2",
                            FILE_TYPE_REGULAR, &level2_a2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(level2_a2);
  dir_node_t *level2_b1 = NULL;

  result = file_tree_create("level2_b1", "flash:/level1_b/level2_b1",
                            FILE_TYPE_REGULAR, &level2_b1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(level2_b1);
  dir_node_t *level3_b1a = NULL;

  result =
      file_tree_create("level3_b1a", "flash:/level1_b/level2_b1/level3_b1a",
                       FILE_TYPE_BINARY, &level3_b1a);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(level3_b1a);

  ASSERT_NOT_NULL(root);
  ASSERT_NOT_NULL(level1_a);
  ASSERT_NOT_NULL(level1_b);
  ASSERT_NOT_NULL(level2_a1);
  ASSERT_NOT_NULL(level2_a2);
  ASSERT_NOT_NULL(level2_b1);
  ASSERT_NOT_NULL(level3_b1a);

  // Build the tree structure
  result = file_tree_add_child(root, level1_a);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(root, level1_b);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(level1_a, level2_a1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(level1_a, level2_a2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(level1_b, level2_b1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(level2_b1, level3_b1a);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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
  result = file_tree_free(root);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // If we reach here without crashing, the test passes
  return 1;
}

TEST(file_tree_free_null_node) {
  setup_file_tree_free_tests();

  // Test freeing a NULL node - this should not cause any crashes
  int result = file_tree_free(NULL);

  ASSERT_EQUAL(FILE_TREE_ERROR_NULL_PARAM, result);

  // If we reach here without crashing, the test passes
  return 1;
}

TEST(file_tree_free_empty_tree) {
  setup_file_tree_free_tests();

  // Create a node with no children
  dir_node_t *node = NULL;

  int result =
      file_tree_create("empty", "flash:/empty", FILE_TYPE_DIRECTORY, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);
  ASSERT_NOT_NULL(node);

  // Verify it has no children
  ASSERT_NULL(node->children);

  // Free the empty tree - this should not cause any crashes
  result = file_tree_free(node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // If we reach here without crashing, the test passes
  return 1;
}

TEST(file_tree_free_nested_structure) {
  setup_file_tree_free_tests();

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
  dir_node_t *level4 = NULL;

  result =
      file_tree_create("level4", "flash:/level0/level1/level2/level3/level4",
                       FILE_TYPE_REGULAR, &level4);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(level4);

  ASSERT_NOT_NULL(level0);
  ASSERT_NOT_NULL(level1);
  ASSERT_NOT_NULL(level2);
  ASSERT_NOT_NULL(level3);
  ASSERT_NOT_NULL(level4);

  // Build the nested structure
  result = file_tree_add_child(level0, level1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(level1, level2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(level2, level3);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(level3, level4);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Verify the nested structure
  ASSERT_EQUAL(level1, level0->children);
  ASSERT_EQUAL(level2, level1->children);
  ASSERT_EQUAL(level3, level2->children);
  ASSERT_EQUAL(level4, level3->children);
  ASSERT_NULL(level4->children);

  // Free the nested structure - this should free all nodes recursively
  result = file_tree_free(level0);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // If we reach here without crashing, the test passes
  return 1;
}

TEST(file_tree_free_mixed_types) {
  setup_file_tree_free_tests();

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
  result = file_tree_free(root);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // If we reach here without crashing, the test passes
  return 1;
}

int main(void) {
  printf("Running file_tree unit tests...\n");

  int result = run_all_tests();

  return result ? 0 : 1;
}
