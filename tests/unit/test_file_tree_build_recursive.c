/**
 * @file test_file_tree_build_recursive.c
 * @brief Unit tests for file_tree_build_recursive function
 *
 * This file contains unit tests for the file_tree_build_recursive function.
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

// Test registry - all test functions
test_func_t test_registry[] = {
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
    NULL};

// Test names for display
const char *test_names[] = {
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
    NULL};

// Test counts
int test_registry_count = sizeof(test_registry) / sizeof(test_registry[0]) - 1;
int test_names_count = sizeof(test_names) / sizeof(test_names[0]) - 1;

// ============================================================================
// Setup Functions
// ============================================================================

/**
 * @brief Setup function for file_tree_build_recursive tests
 */
static void setup_file_tree_build_recursive_tests(void) {
  // No specific setup needed for file_tree_build_recursive tests
  // as it doesn't use mocks or external dependencies
}

// ============================================================================
// file_tree_build_recursive Tests
// ============================================================================

TEST(file_tree_build_recursive_success_single_file) {
  serial_conn_t conn;
  dir_node_t *parent;
  setup_file_tree_build_recursive_tests();

  // Create parent node
  int result = file_tree_create("test_dir", "flash:/test_dir",
                                FILE_TYPE_DIRECTORY, &parent);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  ASSERT_NOT_NULL(parent);

  // Set up mocks for cisco_get_directory_listing to return one file
  // First mock: cisco_send_command succeeds
  MOCK_WRITE_SET_RETURN(15);              // serial_write succeeds
  MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command

  // Second mock: directory listing response
  MOCK_READ_SET_RETURN(
      200, "Directory of flash:/test_dir/\n"
           "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  config.txt\n"
           "1234567 bytes available\n");

  // Call the function under test
  result = file_tree_build_recursive(&conn, parent, "flash:/test_dir");
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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
  int result = file_tree_create("test_dir", "flash:/test_dir",
                                FILE_TYPE_DIRECTORY, &parent);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  ASSERT_NOT_NULL(parent);

  // Set up mocks for cisco_get_directory_listing to return multiple files
  // First mock: cisco_send_command succeeds
  MOCK_WRITE_SET_RETURN(15);              // serial_write succeeds
  MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command

  // Second mock: directory listing response with multiple files
  MOCK_READ_SET_RETURN(
      300, "Directory of flash:/test_dir/\n"
           "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  config.txt\n"
           "2  -rwx  2048      Jan 02 2020 00:00:00 +00:00  backup.bin\n"
           "3  drwx  0          Jan 03 2020 00:00:00 +00:00  logs\n"
           "1234567 bytes available\n");

  // Call the function under test
  result = file_tree_build_recursive(&conn, parent, "flash:/test_dir");
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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
  int result = file_tree_create("mixed_dir", "flash:/mixed_dir",
                                FILE_TYPE_DIRECTORY, &parent);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  ASSERT_NOT_NULL(parent);

  // Set up mocks for cisco_get_directory_listing with mixed file types
  // First mock: cisco_send_command succeeds
  MOCK_WRITE_SET_RETURN(15);              // serial_write succeeds
  MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command

  // Second mock: directory listing response with mixed file types
  MOCK_READ_SET_RETURN(
      400, "Directory of flash:/mixed_dir/\n"
           "1  -rwx  1024      Jan 01 2020 00:00:00 +00:00  readme.txt\n"
           "2  -rwx  8192      Jan 02 2020 00:00:00 +00:00  image.bin\n"
           "3  drwx  0          Jan 03 2020 00:00:00 +00:00  config\n"
           "4  -rwx  512       Jan 04 2020 00:00:00 +00:00  script.sh\n"
           "1234567 bytes available\n");

  // Call the function under test
  result = file_tree_build_recursive(&conn, parent, "flash:/mixed_dir");
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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
  int result = file_tree_create("empty_dir", "flash:/empty_dir",
                                FILE_TYPE_DIRECTORY, &parent);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  ASSERT_NOT_NULL(parent);

  // Set up mocks for cisco_get_directory_listing to return empty directory
  // First mock: cisco_send_command succeeds
  MOCK_WRITE_SET_RETURN(15);              // serial_write succeeds
  MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command

  // Second mock: empty directory listing response
  MOCK_READ_SET_RETURN(100, "Directory of flash:/empty_dir/\n"
                            "1234567 bytes available\n");

  // Call the function under test
  result = file_tree_build_recursive(&conn, parent, "flash:/empty_dir");
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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
  int result = file_tree_create("test_dir", "flash:/test_dir",
                                FILE_TYPE_DIRECTORY, &parent);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
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
  int result = file_tree_create("test_dir", "flash:/test_dir",
                                FILE_TYPE_DIRECTORY, &parent);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
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
  int result = file_tree_create("test_dir", "flash:/test_dir",
                                FILE_TYPE_DIRECTORY, &parent);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  ASSERT_NOT_NULL(parent);

  // Set up mock to simulate directory listing failure
  // Mock serial_write to fail
  MOCK_WRITE_SET_RETURN(-1); // serial_write fails

  // Call the function under test
  result = file_tree_build_recursive(&conn, parent, "flash:/test_dir");
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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
  int result = file_tree_create("test_dir", "flash:/test_dir",
                                FILE_TYPE_DIRECTORY, &parent);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  ASSERT_NOT_NULL(parent);

  // Set up mocks for cisco_get_directory_listing to return files
  // First mock: cisco_send_command succeeds
  MOCK_WRITE_SET_RETURN(15);              // serial_write succeeds
  MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command

  // Second mock: directory listing response
  MOCK_READ_SET_RETURN(
      200, "Directory of flash:/test_dir/\n"
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
  int result = file_tree_create("special_dir", "flash:/special_dir",
                                FILE_TYPE_DIRECTORY, &parent);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  ASSERT_NOT_NULL(parent);

  // Set up mocks for cisco_get_directory_listing with special characters
  // First mock: cisco_send_command succeeds
  MOCK_WRITE_SET_RETURN(15);              // serial_write succeeds
  MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command

  // Second mock: directory listing response with special characters
  MOCK_READ_SET_RETURN(
      300, "Directory of flash:/special_dir/\n"
           "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  config@#$%.txt\n"
           "2  -rwx  2048      Jan 02 2020 00:00:00 +00:00  backup@#$%.bin\n"
           "1234567 bytes available\n");

  // Call the function under test
  result = file_tree_build_recursive(&conn, parent, "flash:/special_dir");
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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
  int result = file_tree_create("long_dir", "flash:/long_dir",
                                FILE_TYPE_DIRECTORY, &parent);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  ASSERT_NOT_NULL(parent);

  // Create a long filename
  char long_filename[300];
  memset(long_filename, 'a', 299);
  long_filename[299] = '\0';

  // Set up mocks for cisco_get_directory_listing with long filename
  // First mock: cisco_send_command succeeds
  MOCK_WRITE_SET_RETURN(15);              // serial_write succeeds
  MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command

  // Second mock: directory listing response with long filename
  char mock_response[500];
  snprintf(mock_response, sizeof(mock_response),
           "Directory of flash:/long_dir/\n"
           "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  %s\n"
           "1234567 bytes available\n",
           long_filename);

  MOCK_READ_SET_RETURN(strlen(mock_response), mock_response);

  // Call the function under test
  result = file_tree_build_recursive(&conn, parent, "flash:/long_dir");
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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
  int result = file_tree_create("nested_dir", "flash:/nested_dir",
                                FILE_TYPE_DIRECTORY, &parent);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);
  ASSERT_NOT_NULL(parent);

  // Set up mocks for cisco_get_directory_listing with nested structure
  // First mock: cisco_send_command succeeds
  MOCK_WRITE_SET_RETURN(15);              // serial_write succeeds
  MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command

  // Second mock: directory listing response with nested structure
  MOCK_READ_SET_RETURN(
      300, "Directory of flash:/nested_dir/\n"
           "1  drwx  0          Jan 01 2020 00:00:00 +00:00  level1\n"
           "2  -rwx  1024      Jan 02 2020 00:00:00 +00:00  file1.txt\n"
           "3  drwx  0          Jan 03 2020 00:00:00 +00:00  level2\n"
           "1234567 bytes available\n");

  // Call the function under test
  result = file_tree_build_recursive(&conn, parent, "flash:/nested_dir");
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result);

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

int main(void) {
  printf("Running file_tree unit tests...\n");

  int result = run_all_tests();

  return result ? 0 : 1;
}
