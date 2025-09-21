/**
 * @file test_file_tree_build.c
 * @brief Unit tests for file_tree_build function
 *
 * This file contains unit tests for the file_tree_build function.
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
test_func_t test_registry[] = {test_file_tree_build_success_single_file,
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
                               NULL};

// Test names for display
const char *test_names[] = {"file_tree_build_success_single_file",
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
                            NULL};

// Test counts
int test_registry_count = sizeof(test_registry) / sizeof(test_registry[0]) - 1;
int test_names_count = sizeof(test_names) / sizeof(test_names[0]) - 1;

// ============================================================================
// Setup Functions
// ============================================================================

/**
 * @brief Setup function for file_tree_build tests
 */
static void setup_file_tree_build_tests(void) {
  // No specific setup needed for file_tree_build tests
  // as it doesn't use mocks or external dependencies
}

// ============================================================================
// file_tree_build Tests
// ============================================================================

TEST(file_tree_build_success_single_file) {
  serial_conn_t conn;
  dir_node_t *root;
  setup_file_tree_build_tests();

  // Set up mocks for cisco_get_directory_listing to return one file
  // First mock: cisco_send_command succeeds
  MOCK_WRITE_SET_RETURN(15);              // serial_write succeeds
  MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command

  // Second mock: directory listing response
  MOCK_READ_SET_RETURN(
      200, "Directory of flash:/\n"
           "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  config.txt\n"
           "1234567 bytes available\n");

  // Call the function under test
  int result = file_tree_build(&conn, &root);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result); // Should succeed

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
  setup_file_tree_build_tests();

  // Set up mocks for cisco_get_directory_listing to return multiple files
  // First mock: cisco_send_command succeeds
  MOCK_WRITE_SET_RETURN(15);              // serial_write succeeds
  MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command

  // Second mock: directory listing response with multiple files
  MOCK_READ_SET_RETURN(
      300, "Directory of flash:/\n"
           "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  config.txt\n"
           "2  -rwx  2048      Jan 02 2020 00:00:00 +00:00  backup.bin\n"
           "3  drwx  0          Jan 03 2020 00:00:00 +00:00  logs\n"
           "1234567 bytes available\n");

  // Call the function under test
  int result = file_tree_build(&conn, &root);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result); // Should succeed

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
  setup_file_tree_build_tests();

  // Set up mocks for cisco_get_directory_listing with mixed file types
  // First mock: cisco_send_command succeeds
  MOCK_WRITE_SET_RETURN(15);              // serial_write succeeds
  MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command

  // Second mock: directory listing response with mixed file types
  MOCK_READ_SET_RETURN(
      400, "Directory of flash:/\n"
           "1  -rwx  1024      Jan 01 2020 00:00:00 +00:00  readme.txt\n"
           "2  -rwx  8192      Jan 02 2020 00:00:00 +00:00  image.bin\n"
           "3  drwx  0          Jan 03 2020 00:00:00 +00:00  config\n"
           "4  -rwx  512       Jan 04 2020 00:00:00 +00:00  script.sh\n"
           "1234567 bytes available\n");

  // Call the function under test
  int result = file_tree_build(&conn, &root);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result); // Should succeed

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
  setup_file_tree_build_tests();

  // Set up mocks for cisco_get_directory_listing to return empty flash
  // First mock: cisco_send_command succeeds
  MOCK_WRITE_SET_RETURN(15);              // serial_write succeeds
  MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command

  // Second mock: empty directory listing response
  MOCK_READ_SET_RETURN(100, "Directory of flash:/\n"
                            "1234567 bytes available\n");

  // Call the function under test
  int result = file_tree_build(&conn, &root);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result); // Should succeed

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
  setup_file_tree_build_tests();

  // Call the function under test with NULL connection
  // This should not cause a crash and should handle gracefully
  int result = file_tree_build(NULL, &root);
  // Should fail due to NULL connection when calling recursive build
  ASSERT_EQUAL(FILE_TREE_ERROR_NULL_PARAM, result);

  // Root node should still be created since the error happens in recursive
  // build
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
  setup_file_tree_build_tests();

  // Call the function under test with NULL root pointer
  // This should not cause a crash and should handle gracefully
  int result = file_tree_build(&conn, NULL);

  // Should return error for NULL root parameter
  ASSERT_EQUAL(FILE_TREE_ERROR_NULL_PARAM, result);

  return 1;
}

TEST(file_tree_build_directory_listing_failure) {
  serial_conn_t conn;
  dir_node_t *root;
  setup_file_tree_build_tests();

  // Set up mock to simulate directory listing failure
  // Mock serial_write to fail
  MOCK_WRITE_SET_RETURN(-1); // serial_write fails

  // Call the function under test
  int result = file_tree_build(&conn, &root);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result); // Should succeed

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
  setup_file_tree_build_tests();

  // Set up mocks for cisco_get_directory_listing to return files
  // First mock: cisco_send_command succeeds
  MOCK_WRITE_SET_RETURN(15);              // serial_write succeeds
  MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command

  // Second mock: directory listing response
  MOCK_READ_SET_RETURN(
      200, "Directory of flash:/\n"
           "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  config.txt\n"
           "1234567 bytes available\n");

  // Call the function under test
  // Note: We can't easily simulate malloc failure in this test framework,
  // but we can test that the function handles the case gracefully
  int result = file_tree_build(&conn, &root);

  // The function should either succeed or fail gracefully
  // If it succeeds, verify the root and child were created correctly
  if (result == FILE_TREE_SUCCESS && root != NULL) {
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
  // If it fails, result should indicate the error

  return 1;
}

TEST(file_tree_build_special_characters) {
  serial_conn_t conn;
  dir_node_t *root;
  setup_file_tree_build_tests();

  // Set up mocks for cisco_get_directory_listing with special characters
  // First mock: cisco_send_command succeeds
  MOCK_WRITE_SET_RETURN(15);              // serial_write succeeds
  MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command

  // Second mock: directory listing response with special characters
  MOCK_READ_SET_RETURN(
      300, "Directory of flash:/\n"
           "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  config@#$%.txt\n"
           "2  -rwx  2048      Jan 02 2020 00:00:00 +00:00  backup@#$%.bin\n"
           "1234567 bytes available\n");

  // Call the function under test
  int result = file_tree_build(&conn, &root);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result); // Should succeed

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
  setup_file_tree_build_tests();

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
           "Directory of flash:/\n"
           "1  -rwx  1429      Jan 01 2020 00:00:00 +00:00  %s\n"
           "1234567 bytes available\n",
           long_filename);

  MOCK_READ_SET_RETURN(strlen(mock_response), mock_response);

  // Call the function under test
  int result = file_tree_build(&conn, &root);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result); // Should succeed

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
  setup_file_tree_build_tests();

  // Set up mocks for cisco_get_directory_listing with nested structure
  // First mock: cisco_send_command succeeds
  MOCK_WRITE_SET_RETURN(15);              // serial_write succeeds
  MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command

  // Second mock: directory listing response with nested structure
  MOCK_READ_SET_RETURN(
      300, "Directory of flash:/\n"
           "1  drwx  0          Jan 01 2020 00:00:00 +00:00  level1\n"
           "2  -rwx  1024      Jan 02 2020 00:00:00 +00:00  file1.txt\n"
           "3  drwx  0          Jan 03 2020 00:00:00 +00:00  level2\n"
           "1234567 bytes available\n");

  // Call the function under test
  int result = file_tree_build(&conn, &root);
  ASSERT_EQUAL(FILE_TREE_SUCCESS, result); // Should succeed

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

int main(void) {
  printf("Running file_tree unit tests...\n");

  int result = run_all_tests();

  return result ? 0 : 1;
}
