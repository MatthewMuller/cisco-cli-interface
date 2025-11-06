/**
 * @file test_file_tree_add_child.c
 * @brief Unit tests for file_tree_add_child function
 *
 * This file contains unit tests for the file_tree_add_child function:
 * - Success cases for adding single and multiple children
 * - Edge cases with NULL parameters
 * - Level inheritance verification
 * - Parent pointer setting
 * - Linked list order maintenance
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
    test_file_tree_add_child_success_first_child,
    test_file_tree_add_child_success_multiple_children,
    test_file_tree_add_child_null_parent,
    test_file_tree_add_child_null_child,
    test_file_tree_add_child_both_null,
    test_file_tree_add_child_level_inheritance,
    test_file_tree_add_child_parent_pointer_set,
    test_file_tree_add_child_linked_list_order,
    NULL};

// Test names for display
const char *test_names[] = {"file_tree_add_child_success_first_child",
                            "file_tree_add_child_success_multiple_children",
                            "file_tree_add_child_null_parent",
                            "file_tree_add_child_null_child",
                            "file_tree_add_child_both_null",
                            "file_tree_add_child_level_inheritance",
                            "file_tree_add_child_parent_pointer_set",
                            "file_tree_add_child_linked_list_order",
                            NULL};

// Test counts
int test_registry_count = sizeof(test_registry) / sizeof(test_registry[0]) - 1;
int test_names_count = sizeof(test_names) / sizeof(test_names[0]) - 1;

// ============================================================================
// Setup Functions
// ============================================================================

/**
 * @brief Setup function for file_tree_add_child tests
 */
static void setup_file_tree_add_child_tests(void) {
  // No specific setup needed for file_tree_add_child tests
  // as it doesn't use mocks or external dependencies
}

// ============================================================================
// file_tree_add_child Tests
// ============================================================================

TEST(file_tree_add_child_success_first_child) {
  setup_file_tree_add_child_tests();

  // Create parent and child nodes
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

  // Verify nodes were created successfully
  ASSERT_NOT_NULL(parent);
  ASSERT_NOT_NULL(child);

  // Add child to parent
  result = file_tree_add_child(parent, child);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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
  setup_file_tree_add_child_tests();

  // Create parent and multiple child nodes
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

  // Verify nodes were created successfully
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
  setup_file_tree_add_child_tests();

  // Create child node
  dir_node_t *child = NULL;

  int result =
      file_tree_create("child", "flash:/child", FILE_TYPE_REGULAR, &child);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child);

  // Verify child was created successfully
  ASSERT_NOT_NULL(child);

  // Store original values
  dir_node_t *original_parent = child->parent;
  int original_level = child->level;

  // Try to add child to NULL parent
  result = file_tree_add_child(NULL, child);
  ASSERT_EQUAL(FILE_TREE_ERROR_NULL_PARAM, result);

  // Verify child remains unchanged (function should handle NULL parent
  // gracefully)
  ASSERT_EQUAL(original_parent, child->parent);
  ASSERT_EQUAL(original_level, child->level);

  // Clean up
  free(child);

  return 1;
}

TEST(file_tree_add_child_null_child) {
  setup_file_tree_add_child_tests();

  // Create parent node
  dir_node_t *parent = NULL;

  int result =
      file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY, &parent);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(parent);

  // Verify parent was created successfully
  ASSERT_NOT_NULL(parent);

  // Store original children pointer
  dir_node_t *original_children = parent->children;

  // Try to add NULL child to parent
  result = file_tree_add_child(parent, NULL);
  ASSERT_EQUAL(FILE_TREE_ERROR_NULL_PARAM, result);

  // Verify parent's children remain unchanged (function should handle NULL
  // child gracefully)
  ASSERT_EQUAL(original_children, parent->children);

  // Clean up
  free(parent);

  return 1;
}

TEST(file_tree_add_child_both_null) {
  setup_file_tree_add_child_tests();

  // Try to add NULL child to NULL parent
  // This should not cause any crashes or errors
  int result = file_tree_add_child(NULL, NULL);
  ASSERT_EQUAL(FILE_TREE_ERROR_NULL_PARAM, result);

  // If we reach here without crashing, the test passes
  return 1;
}

TEST(file_tree_add_child_level_inheritance) {
  setup_file_tree_add_child_tests();

  // Create a multi-level tree structure
  dir_node_t *root = NULL;

  int result = file_tree_create("root", "flash:/", FILE_TYPE_DIRECTORY, &root);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(root);
  dir_node_t *level1 = NULL;

  result =
      file_tree_create("level1", "flash:/level1", FILE_TYPE_DIRECTORY, &level1);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(level1);
  dir_node_t *level2 = NULL;

  result = file_tree_create("level2", "flash:/level1/level2", FILE_TYPE_REGULAR,
                            &level2);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(level2);

  // Verify nodes were created successfully
  ASSERT_NOT_NULL(root);
  ASSERT_NOT_NULL(level1);
  ASSERT_NOT_NULL(level2);

  // Set up initial levels
  root->level = 0;
  level1->level = 1;

  // Add level1 to root
  result = file_tree_add_child(root, level1);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Verify level1's level is incremented from root
  ASSERT_EQUAL(1, level1->level);

  // Add level2 to level1
  result = file_tree_add_child(level1, level2);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Verify level2's level is incremented from level1
  ASSERT_EQUAL(2, level2->level);

  // Clean up
  file_tree_free(root);

  return 1;
}

TEST(file_tree_add_child_parent_pointer_set) {
  setup_file_tree_add_child_tests();

  // Create parent and child nodes
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

  // Verify nodes were created successfully
  ASSERT_NOT_NULL(parent);
  ASSERT_NOT_NULL(child);

  // Verify child's parent is initially NULL
  ASSERT_NULL(child->parent);

  // Add child to parent
  result = file_tree_add_child(parent, child);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  // Verify child's parent pointer is now set to parent
  ASSERT_EQUAL(parent, child->parent);

  // Clean up
  file_tree_free(parent);

  return 1;
}

TEST(file_tree_add_child_linked_list_order) {
  setup_file_tree_add_child_tests();

  // Create parent and multiple child nodes
  dir_node_t *parent = NULL;

  int result =
      file_tree_create("parent", "flash:/parent", FILE_TYPE_DIRECTORY, &parent);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(parent);
  dir_node_t *child_a = NULL;

  result = file_tree_create("child_a", "flash:/parent/child_a",
                            FILE_TYPE_REGULAR, &child_a);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child_a);
  dir_node_t *child_b = NULL;

  result = file_tree_create("child_b", "flash:/parent/child_b",
                            FILE_TYPE_REGULAR, &child_b);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child_b);
  dir_node_t *child_c = NULL;

  result = file_tree_create("child_c", "flash:/parent/child_c",
                            FILE_TYPE_REGULAR, &child_c);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(child_c);

  // Verify nodes were created successfully
  ASSERT_NOT_NULL(parent);
  ASSERT_NOT_NULL(child_a);
  ASSERT_NOT_NULL(child_b);
  ASSERT_NOT_NULL(child_c);

  // Add children in specific order
  result = file_tree_add_child(parent, child_a);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(parent, child_b);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  result = file_tree_add_child(parent, child_c);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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

int main(void) {
  printf("Running file_tree unit tests...\n");

  int result = run_all_tests();

  return result ? 0 : 1;
}
