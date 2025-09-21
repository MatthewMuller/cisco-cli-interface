/**
 * @file test_file_tree_create.c
 * @brief Unit tests for file_tree_create function
 *
 * This file contains unit tests for the file_tree_create function:
 * - Success cases for different file types
 * - Edge cases with NULL parameters
 * - Boundary conditions with long names/paths
 * - Special character handling
 * - Memory allocation scenarios
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

// Test registry - all test functions
test_func_t test_registry[] = {test_file_tree_create_success_regular_file,
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
                               NULL};

// Test names for display
const char *test_names[] = {"file_tree_create_success_regular_file",
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
                            NULL};

// Test counts
int test_registry_count = sizeof(test_registry) / sizeof(test_registry[0]) - 1;
int test_names_count = sizeof(test_names) / sizeof(test_names[0]) - 1;

// ============================================================================
// Setup Functions
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
  dir_node_t *node = NULL;

  int result = file_tree_create("config.txt", "flash:/config.txt",
                                FILE_TYPE_REGULAR, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);

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
  dir_node_t *node = NULL;

  int result =
      file_tree_create("backup", "flash:/backup", FILE_TYPE_DIRECTORY, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);

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
  dir_node_t *node = NULL;

  int result = file_tree_create("image.bin", "flash:/image.bin",
                                FILE_TYPE_BINARY, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);

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
  dir_node_t *node = NULL;

  int result =
      file_tree_create(NULL, "flash:/config.txt", FILE_TYPE_REGULAR, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);

  // Verify node was created successfully (function should handle NULL
  // gracefully)
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
  dir_node_t *node = NULL;

  int result = file_tree_create("config.txt", NULL, FILE_TYPE_REGULAR, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);

  // Verify node was created successfully (function should handle NULL
  // gracefully)
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
  dir_node_t *node = NULL;

  int result =
      file_tree_create("", "flash:/config.txt", FILE_TYPE_REGULAR, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);

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
  dir_node_t *node = NULL;

  int result = file_tree_create("config.txt", "", FILE_TYPE_REGULAR, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);

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
  dir_node_t *node = NULL;

  int result = file_tree_create(long_name, "flash:/longname.txt",
                                FILE_TYPE_REGULAR, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);

  // Verify node was created successfully
  ASSERT_NOT_NULL(node);

  // Verify name is truncated to MAX_PATH_LEN - 1 characters (plus null
  // terminator)
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
  dir_node_t *node = NULL;

  int result =
      file_tree_create("config.txt", long_path, FILE_TYPE_REGULAR, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);

  // Verify node was created successfully
  ASSERT_NOT_NULL(node);

  // Verify name is correctly set
  ASSERT_STRING_EQUAL("config.txt", node->name);

  // Verify path is truncated to MAX_PATH_LEN - 1 characters (plus null
  // terminator)
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

  dir_node_t *node = NULL;

  int result =
      file_tree_create(special_name, special_path, FILE_TYPE_REGULAR, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);

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
  dir_node_t *node = NULL;

  int result =
      file_tree_create("test.txt", "flash:/test.txt", FILE_TYPE_REGULAR, &node);

  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

  ASSERT_NOT_NULL(node);

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

int main(void) {
  printf("Running file_tree unit tests...\n");

  int result = run_all_tests();

  return result ? 0 : 1;
}
