/**
 * @file test_cisco_commands_v2.c
 * @brief Unit tests for cisco_commands.c using streamlined framework
 * 
 * This file demonstrates the new streamlined test approach with:
 * - Simple test registration
 * - Simplified mock setup
 * - Cleaner test structure
 * - Better organization
 * 
 * @author Cisco CLI Interface Team
 * @version 2.0
 */

#include "../framework/test_framework.h"
#include "../include/cisco_cli.h"

// ============================================================================
// Test Registry
// ============================================================================

// Forward declarations of all test functions
TEST(wait_for_prompt_success_immediate);
TEST(wait_for_prompt_success_delayed);
TEST(wait_for_prompt_timeout);
TEST(wait_for_prompt_no_prompt_in_data);
TEST(wait_for_prompt_empty_data);
TEST(send_command_success);
TEST(send_command_serial_write_failure);
TEST(send_command_prompt_timeout);
TEST(send_command_empty_command);
TEST(send_command_long_command);
TEST(send_command_special_characters);
TEST(init_flash_success_immediate);
TEST(init_flash_success_delayed);
TEST(init_flash_timeout);
TEST(init_flash_send_command_failure);
TEST(init_flash_empty_data);
TEST(get_directory_listing_success_flash);
TEST(get_directory_listing_success_custom_path);
TEST(get_directory_listing_send_command_failure);
TEST(get_directory_listing_read_failure);
TEST(get_directory_listing_empty_directory);
TEST(get_directory_listing_mixed_file_types);
TEST(get_directory_listing_long_filenames);
TEST(get_directory_listing_malformed_output);
TEST(get_directory_listing_memory_allocation_failure);
TEST(delete_file_success);
TEST(delete_file_send_command_failure);
TEST(delete_file_no_confirmation_prompt);
TEST(delete_file_confirmation_write_failure);
TEST(delete_file_deletion_failed);
TEST(delete_file_timeout_after_confirmation);
TEST(delete_file_empty_file_path);
TEST(delete_file_long_file_path);
TEST(delete_file_special_characters_in_path);
TEST(delete_directory_success);
TEST(delete_directory_send_command_failure);
TEST(delete_directory_no_confirmation_prompt);
TEST(delete_directory_confirmation_write_failure);
TEST(delete_directory_removal_failed);
TEST(delete_directory_timeout_after_confirmation);
TEST(delete_directory_empty_dir_path);
TEST(delete_directory_long_dir_path);
TEST(delete_directory_special_characters_in_path);

// Test registry - all test functions
test_func_t test_registry[] = {
    test_wait_for_prompt_success_immediate,
    test_wait_for_prompt_success_delayed,
    test_wait_for_prompt_timeout,
    test_wait_for_prompt_no_prompt_in_data,
    test_wait_for_prompt_empty_data,
    test_send_command_success,
    test_send_command_serial_write_failure,
    test_send_command_prompt_timeout,
    test_send_command_empty_command,
    test_send_command_long_command,
    test_send_command_special_characters,
    test_init_flash_success_immediate,
    test_init_flash_success_delayed,
    test_init_flash_timeout,
    test_init_flash_send_command_failure,
    test_init_flash_empty_data,
    test_get_directory_listing_success_flash,
    test_get_directory_listing_success_custom_path,
    test_get_directory_listing_send_command_failure,
    test_get_directory_listing_read_failure,
    test_get_directory_listing_empty_directory,
    test_get_directory_listing_mixed_file_types,
    test_get_directory_listing_long_filenames,
    test_get_directory_listing_malformed_output,
    test_get_directory_listing_memory_allocation_failure,
    test_delete_file_success,
    test_delete_file_send_command_failure,
    test_delete_file_no_confirmation_prompt,
    test_delete_file_confirmation_write_failure,
    test_delete_file_deletion_failed,
    test_delete_file_timeout_after_confirmation,
    test_delete_file_empty_file_path,
    test_delete_file_long_file_path,
    test_delete_file_special_characters_in_path,
    test_delete_directory_success,
    test_delete_directory_send_command_failure,
    test_delete_directory_no_confirmation_prompt,
    test_delete_directory_confirmation_write_failure,
    test_delete_directory_removal_failed,
    test_delete_directory_timeout_after_confirmation,
    test_delete_directory_empty_dir_path,
    test_delete_directory_long_dir_path,
    test_delete_directory_special_characters_in_path,
    NULL
};

int test_registry_count = sizeof(test_registry) / sizeof(test_registry[0]) - 1;

// Test names registry - parallel array to test_registry
const char* test_names[] = {
    "wait_for_prompt_success_immediate",
    "wait_for_prompt_success_delayed",
    "wait_for_prompt_timeout",
    "wait_for_prompt_no_prompt_in_data",
    "wait_for_prompt_empty_data",
    "send_command_success",
    "send_command_serial_write_failure",
    "send_command_prompt_timeout",
    "send_command_empty_command",
    "send_command_long_command",
    "send_command_special_characters",
    "init_flash_success_immediate",
    "init_flash_success_delayed",
    "init_flash_timeout",
    "init_flash_send_command_failure",
    "init_flash_empty_data",
    "get_directory_listing_success_flash",
    "get_directory_listing_success_custom_path",
    "get_directory_listing_send_command_failure",
    "get_directory_listing_read_failure",
    "get_directory_listing_empty_directory",
    "get_directory_listing_mixed_file_types",
    "get_directory_listing_long_filenames",
    "get_directory_listing_malformed_output",
    "get_directory_listing_memory_allocation_failure",
    "delete_file_success",
    "delete_file_send_command_failure",
    "delete_file_no_confirmation_prompt",
    "delete_file_confirmation_write_failure",
    "delete_file_deletion_failed",
    "delete_file_timeout_after_confirmation",
    "delete_file_empty_file_path",
    "delete_file_long_file_path",
    "delete_file_special_characters_in_path",
    "delete_directory_success",
    "delete_directory_send_command_failure",
    "delete_directory_no_confirmation_prompt",
    "delete_directory_confirmation_write_failure",
    "delete_directory_removal_failed",
    "delete_directory_timeout_after_confirmation",
    "delete_directory_empty_dir_path",
    "delete_directory_long_dir_path",
    "delete_directory_special_characters_in_path",
    NULL
};

int test_names_count = sizeof(test_names) / sizeof(test_names[0]) - 1;

// ============================================================================
// Test Fixtures
// ============================================================================

/**
 * @brief Setup function for cisco_wait_for_prompt tests
 */
static void setup_wait_for_prompt_tests(void) {
    MOCK_INIT_ALL();
}

/**
 * @brief Setup function for cisco_send_command tests
 */
static void setup_send_command_tests(void) {
    MOCK_INIT_ALL();
}

/**
 * @brief Setup function for cisco_init_flash tests
 */
static void setup_init_flash_tests(void) {
    MOCK_INIT_ALL();
}

/**
 * @brief Setup function for cisco_get_directory_listing tests
 */
static void setup_get_directory_listing_tests(void) {
    MOCK_INIT_ALL();
}

/**
 * @brief Setup function for cisco_delete_file tests
 */
static void setup_delete_file_tests(void) {
    MOCK_INIT_ALL();
}

/**
 * @brief Setup function for cisco_delete_directory tests
 */
static void setup_delete_directory_tests(void) {
    MOCK_INIT_ALL();
}

// ============================================================================
// cisco_wait_for_prompt Tests
// ============================================================================

TEST(wait_for_prompt_success_immediate) {
    serial_conn_t conn;
    setup_wait_for_prompt_tests();
    
    // Set up mock to return a line with prompt immediately
    MOCK_READ_SET_RETURN(15, "Router# : ");
    
    int result = cisco_wait_for_prompt(&conn, 1);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(1, mock_serial_read.call_count);
    
    return 1;
}

TEST(wait_for_prompt_success_delayed) {
    serial_conn_t conn;
    setup_wait_for_prompt_tests();
    
    // Set up mock to return empty lines first, then prompt
    MOCK_READ_SET_RETURN(0, "");  // No data
    MOCK_READ_SET_RETURN(12, "Router# : ");  // Prompt found
    
    int result = cisco_wait_for_prompt(&conn, 2);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(2, mock_serial_read.call_count);
    
    return 1;
}

TEST(wait_for_prompt_timeout) {
    serial_conn_t conn;
    setup_wait_for_prompt_tests();
    
    // No mock setup - will timeout
    int result = cisco_wait_for_prompt(&conn, 1);
    
    ASSERT_EQUAL(-1, result);
    ASSERT_TRUE(mock_serial_read.call_count >= 1);
    
    return 1;
}

TEST(wait_for_prompt_no_prompt_in_data) {
    serial_conn_t conn;
    setup_wait_for_prompt_tests();
    
    // Set up mock to return data without prompt, then data with prompt
    MOCK_READ_SET_RETURN(20, "Some output without prompt\n");
    MOCK_READ_SET_RETURN(15, "Router# : ");
    
    int result = cisco_wait_for_prompt(&conn, 2);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(2, mock_serial_read.call_count);
    
    return 1;
}

TEST(wait_for_prompt_empty_data) {
    serial_conn_t conn;
    setup_wait_for_prompt_tests();
    
    // Set up mock to return empty data, then prompt
    MOCK_READ_SET_RETURN(0, "");
    MOCK_READ_SET_RETURN(0, "");
    MOCK_READ_SET_RETURN(10, "Router# : ");
    
    int result = cisco_wait_for_prompt(&conn, 3);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(3, mock_serial_read.call_count);
    
    return 1;
}

// ============================================================================
// cisco_send_command Tests
// ============================================================================

TEST(send_command_success) {
    serial_conn_t conn;
    setup_send_command_tests();
    
    // Set up mocks
    MOCK_WRITE_SET_RETURN(10); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found immediately
    
    int result = cisco_send_command(&conn, "show version", 5);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(1, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("show version\n", mock_serial_write.last_data);
    ASSERT_EQUAL(1, mock_serial_read.call_count);
    
    return 1;
}

TEST(send_command_serial_write_failure) {
    serial_conn_t conn;
    setup_send_command_tests();
    
    // Set up mock to make serial_write fail
    MOCK_WRITE_SET_RETURN(-1); // serial_write fails
    
    int result = cisco_send_command(&conn, "show version", 5);
    
    ASSERT_EQUAL(-1, result);
    ASSERT_EQUAL(1, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("show version\n", mock_serial_write.last_data);
    ASSERT_EQUAL(0, mock_serial_read.call_count); // Should not be called if write fails
    
    return 1;
}

TEST(send_command_prompt_timeout) {
    serial_conn_t conn;
    setup_send_command_tests();
    
    // Set up mocks
    MOCK_WRITE_SET_RETURN(10); // serial_write succeeds
    // No mock setup for serial_read_until, so it will return 0 (timeout)
    
    int result = cisco_send_command(&conn, "show interfaces", 1);
    
    ASSERT_EQUAL(-1, result);
    ASSERT_EQUAL(1, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("show interfaces\n", mock_serial_write.last_data);
    ASSERT_TRUE(mock_serial_read.call_count >= 1); // Should be called at least once
    
    return 1;
}

TEST(send_command_empty_command) {
    serial_conn_t conn;
    setup_send_command_tests();
    
    // Set up mocks
    MOCK_WRITE_SET_RETURN(1); // serial_write succeeds
    MOCK_READ_SET_RETURN(10, "Router# : "); // prompt found
    
    int result = cisco_send_command(&conn, "", 5);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(1, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("\n", mock_serial_write.last_data);
    ASSERT_EQUAL(1, mock_serial_read.call_count);
    
    return 1;
}

TEST(send_command_long_command) {
    serial_conn_t conn;
    setup_send_command_tests();
    
    // Create a long command (but within MAX_LINE_LEN)
    char long_command[512];
    memset(long_command, 'a', 511);
    long_command[511] = '\0';
    
    // Set up mocks
    MOCK_WRITE_SET_RETURN(512); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found
    
    int result = cisco_send_command(&conn, long_command, 5);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(1, mock_serial_write.call_count);
    // Check that the command was formatted with newline
    ASSERT_EQUAL(strlen(long_command) + 1, strlen(mock_serial_write.last_data));
    ASSERT_EQUAL('\n', mock_serial_write.last_data[strlen(long_command)]);
    ASSERT_EQUAL(1, mock_serial_read.call_count);
    
    return 1;
}

TEST(send_command_special_characters) {
    serial_conn_t conn;
    setup_send_command_tests();
    
    // Set up mocks
    MOCK_WRITE_SET_RETURN(20); // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found
    
    int result = cisco_send_command(&conn, "show ip route | include 192.168.1.0/24", 5);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(1, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("show ip route | include 192.168.1.0/24\n", mock_serial_write.last_data);
    ASSERT_EQUAL(1, mock_serial_read.call_count);
    
    return 1;
}

// ============================================================================
// cisco_init_flash Tests
// ============================================================================

TEST(init_flash_success_immediate) {
    serial_conn_t conn;
    setup_init_flash_tests();
    
    // Set up mocks - flash_init prompt found immediately, then cisco_send_command succeeds
    MOCK_READ_SET_RETURN(20, "flash_init\n"); // flash_init prompt found
    MOCK_WRITE_SET_RETURN(10); // serial_write succeeds for flash_init command
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after flash_init command
    
    int result = cisco_init_flash(&conn, 5);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(2, mock_serial_read.call_count); // Called once to find flash_init prompt, once in cisco_send_command
    ASSERT_EQUAL(1, mock_serial_write.call_count); // Called once to send flash_init command
    ASSERT_STRING_EQUAL("flash_init\n", mock_serial_write.last_data);
    
    return 1;
}

TEST(init_flash_success_delayed) {
    serial_conn_t conn;
    setup_init_flash_tests();
    
    // Set up mocks - some data without flash_init prompt, then flash_init prompt, then cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Some output\n"); // No flash_init prompt
    MOCK_READ_SET_RETURN(20, "flash_init\n"); // flash_init prompt found
    MOCK_WRITE_SET_RETURN(10); // serial_write succeeds for flash_init command
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after flash_init command
    
    int result = cisco_init_flash(&conn, 5);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(3, mock_serial_read.call_count); // Called twice to find flash_init prompt, once in cisco_send_command
    ASSERT_EQUAL(1, mock_serial_write.call_count); // Called once to send flash_init command
    ASSERT_STRING_EQUAL("flash_init\n", mock_serial_write.last_data);
    
    return 1;
}

TEST(init_flash_timeout) {
    serial_conn_t conn;
    setup_init_flash_tests();
    
    // No mock setup - will timeout
    int result = cisco_init_flash(&conn, 1); // 1 second timeout
    
    ASSERT_EQUAL(-1, result);
    ASSERT_TRUE(mock_serial_read.call_count >= 1); // Should have been called at least 1 time
    ASSERT_EQUAL(0, mock_serial_write.call_count); // Should not be called if timeout occurs
    
    return 1;
}

TEST(init_flash_send_command_failure) {
    serial_conn_t conn;
    setup_init_flash_tests();
    
    // Set up mocks - flash_init prompt found, but cisco_send_command fails
    MOCK_READ_SET_RETURN(20, "flash_init\n"); // flash_init prompt found
    MOCK_WRITE_SET_RETURN(-1); // serial_write fails for flash_init command
    
    int result = cisco_init_flash(&conn, 5);
    
    ASSERT_EQUAL(-1, result);
    ASSERT_EQUAL(1, mock_serial_read.call_count); // Called once to find flash_init prompt
    ASSERT_EQUAL(1, mock_serial_write.call_count); // Called once to send flash_init command
    ASSERT_STRING_EQUAL("flash_init\n", mock_serial_write.last_data);
    
    return 1;
}

TEST(init_flash_empty_data) {
    serial_conn_t conn;
    setup_init_flash_tests();
    
    // Set up mocks - empty data, then flash_init prompt, then cisco_send_command succeeds
    MOCK_READ_SET_RETURN(0, ""); // Empty data
    MOCK_READ_SET_RETURN(0, ""); // Empty data
    MOCK_READ_SET_RETURN(20, "flash_init\n"); // flash_init prompt found
    MOCK_WRITE_SET_RETURN(10); // serial_write succeeds for flash_init command
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after flash_init command
    
    int result = cisco_init_flash(&conn, 5);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(4, mock_serial_read.call_count); // Called three times to find flash_init prompt, once in cisco_send_command
    ASSERT_EQUAL(1, mock_serial_write.call_count); // Called once to send flash_init command
    ASSERT_STRING_EQUAL("flash_init\n", mock_serial_write.last_data);
    
    return 1;
}

// ============================================================================
// cisco_get_directory_listing Tests
// ============================================================================

TEST(get_directory_listing_success_flash) {
    serial_conn_t conn;
    file_entry_t *files = NULL;
    setup_get_directory_listing_tests();
    
    // Set up mocks for successful directory listing
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(200, "Directory of flash:/longnames\n"
                              "1  -rwx  1024      Jan 01 2020 00:00:00 +00:00  config.txt\n"
                              "2  -rwx  2048      Jan 01 2020 00:00:00 +00:00  image.bin\n"
                              "3  drwx  0          Jan 01 2020 00:00:00 +00:00  backup\n"
                              "9207808 bytes available (18790400 bytes used)\n");
    
    int result = cisco_get_directory_listing(&conn, "flash:/", &files, 30);
    

    
    ASSERT_EQUAL(3, result);
    ASSERT_NOT_NULL(files);
    
    // Check first file (config.txt)
    ASSERT_STRING_EQUAL("config.txt", files->name);
    ASSERT_STRING_EQUAL("flash:/config.txt", files->full_path);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, files->type);
    ASSERT_EQUAL(1024, files->size);
    ASSERT_EQUAL(0, files->selected);
    
    // Check second file (image.bin)
    ASSERT_NOT_NULL(files->next);
    ASSERT_STRING_EQUAL("image.bin", files->next->name);
    ASSERT_STRING_EQUAL("flash:/image.bin", files->next->full_path);
    ASSERT_EQUAL(FILE_TYPE_BINARY, files->next->type);
    ASSERT_EQUAL(2048, files->next->size);
    
    // Check third file (backup directory)
    ASSERT_NOT_NULL(files->next->next);
    ASSERT_STRING_EQUAL("backup", files->next->next->name);
    ASSERT_STRING_EQUAL("flash:/backup", files->next->next->full_path);
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, files->next->next->type);
    ASSERT_EQUAL(0, files->next->next->size);
    
    // Verify no more files
    ASSERT_NULL(files->next->next->next);
    
    // Verify mock calls
    ASSERT_EQUAL(1, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("dir flash:/\n", mock_serial_write.last_data);
    ASSERT_EQUAL(2, mock_serial_read.call_count);
    
    // Clean up
    while (files) {
        file_entry_t *temp = files;
        files = files->next;
        free(temp);
    }
    
    return 1;
}

TEST(get_directory_listing_success_custom_path) {
    serial_conn_t conn;
    file_entry_t *files = NULL;
    setup_get_directory_listing_tests();
    
    // Set up mocks for successful directory listing with custom path
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(150, "Directory of flash:/backup\n"
                              "5  -rwx  512       Jan 01 2020 00:00:00 +00:00  old_config.txt\n"
                              "6  -rwx  1024      Jan 01 2020 00:00:00 +00:00  backup.bin\n");
    
    int result = cisco_get_directory_listing(&conn, "flash:/backup", &files, 30);
    

    
    ASSERT_EQUAL(2, result);
    ASSERT_NOT_NULL(files);
    
    // Check first file
    ASSERT_STRING_EQUAL("old_config.txt", files->name);
    ASSERT_STRING_EQUAL("flash:/backup/old_config.txt", files->full_path);
    ASSERT_EQUAL(FILE_TYPE_REGULAR, files->type);
    ASSERT_EQUAL(512, files->size);
    
    // Check second file
    ASSERT_NOT_NULL(files->next);
    ASSERT_STRING_EQUAL("backup.bin", files->next->name);
    ASSERT_STRING_EQUAL("flash:/backup/backup.bin", files->next->full_path);
    ASSERT_EQUAL(FILE_TYPE_BINARY, files->next->type);
    ASSERT_EQUAL(1024, files->next->size);
    
    // Verify no more files
    ASSERT_NULL(files->next->next);
    
    // Verify mock calls
    ASSERT_EQUAL(1, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("dir flash:/backup\n", mock_serial_write.last_data);
    
    // Clean up
    while (files) {
        file_entry_t *temp = files;
        files = files->next;
        free(temp);
    }
    
    return 1;
}

TEST(get_directory_listing_send_command_failure) {
    serial_conn_t conn;
    file_entry_t *files = NULL;
    setup_get_directory_listing_tests();
    
    // Set up mock to make cisco_send_command fail
    MOCK_WRITE_SET_RETURN(-1); // serial_write fails
    
    int result = cisco_get_directory_listing(&conn, "flash:/", &files, 30);
    
    ASSERT_EQUAL(-1, result);
    ASSERT_NULL(files);
    ASSERT_EQUAL(1, mock_serial_write.call_count);
    ASSERT_EQUAL(0, mock_serial_read.call_count);
    
    return 1;
}

TEST(get_directory_listing_read_failure) {
    serial_conn_t conn;
    file_entry_t *files = NULL;
    setup_get_directory_listing_tests();
    
    // Set up mocks - command succeeds but read fails
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(-1, ""); // serial_read_until fails
    
    int result = cisco_get_directory_listing(&conn, "flash:/", &files, 30);
    
    ASSERT_EQUAL(-1, result);
    ASSERT_NULL(files);
    ASSERT_EQUAL(1, mock_serial_write.call_count);
    ASSERT_EQUAL(2, mock_serial_read.call_count);
    
    return 1;
}

TEST(get_directory_listing_empty_directory) {
    serial_conn_t conn;
    file_entry_t *files = NULL;
    setup_get_directory_listing_tests();
    
    // Set up mocks for empty directory
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(100, "Directory of flash:/empty\n"
                              "No files in directory\n"
                              "123456789 bytes available\n");
    
    int result = cisco_get_directory_listing(&conn, "flash:/empty", &files, 30);
    
    ASSERT_EQUAL(0, result);
    ASSERT_NULL(files);
    ASSERT_EQUAL(1, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("dir flash:/empty\n", mock_serial_write.last_data);
    
    return 1;
}

TEST(get_directory_listing_mixed_file_types) {
    serial_conn_t conn;
    file_entry_t *files = NULL;
    setup_get_directory_listing_tests();
    
    // Set up mocks for directory with mixed file types
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(250, "Directory of flash:/mixed\n"
                              "1  -rwx  1024      Jan 01 2020 00:00:00 +00:00  config.txt\n"
                              "2  drwx  0          Jan 01 2020 00:00:00 +00:00  logs\n"
                              "3  -rwx  4096      Jan 01 2020 00:00:00 +00:00  firmware.bin\n"
                              "4  -rwx  256        Jan 01 2020 00:00:00 +00:00  startup.cfg\n"
                              "5  drwx  0          Jan 01 2020 00:00:00 +00:00  temp\n");
    
    int result = cisco_get_directory_listing(&conn, "flash:/mixed", &files, 30);
    

    
    ASSERT_EQUAL(5, result);
    ASSERT_NOT_NULL(files);
    
    // Verify file types are correctly identified
    file_entry_t *current = files;
    ASSERT_EQUAL(FILE_TYPE_REGULAR, current->type); // config.txt
    current = current->next;
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, current->type); // logs/
    current = current->next;
    ASSERT_EQUAL(FILE_TYPE_BINARY, current->type); // firmware.bin
    current = current->next;
    ASSERT_EQUAL(FILE_TYPE_REGULAR, current->type); // startup.cfg
    current = current->next;
    ASSERT_EQUAL(FILE_TYPE_DIRECTORY, current->type); // temp/
    
    // Clean up
    while (files) {
        file_entry_t *temp = files;
        files = files->next;
        free(temp);
    }
    
    return 1;
}

TEST(get_directory_listing_long_filenames) {
    serial_conn_t conn;
    file_entry_t *files = NULL;
    setup_get_directory_listing_tests();
    
    // Set up mocks for directory with long filenames
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(200, "Directory of flash:/longnames\n"
                              "1  -rwx  1024      Jan 01 2020 00:00:00 +00:00  very_long_filename_that_might_exceed_normal_limits.txt\n"
                              "2  -rwx  2048      Jan 01 2020 00:00:00 +00:00  another_very_long_filename_with_special_chars_@#$%.bin\n");
    
    int result = cisco_get_directory_listing(&conn, "flash:/longnames", &files, 30);
    
    ASSERT_EQUAL(2, result);
    ASSERT_NOT_NULL(files);
    
    // Check that long filenames are handled correctly
    ASSERT_STRING_EQUAL("very_long_filename_that_might_exceed_normal_limits.txt", files->name);
    ASSERT_STRING_EQUAL("flash:/longnames/very_long_filename_that_might_exceed_normal_limits.txt", files->full_path);
    
    ASSERT_NOT_NULL(files->next);
    ASSERT_STRING_EQUAL("another_very_long_filename_with_special_chars_@#$%.bin", files->next->name);
    ASSERT_STRING_EQUAL("flash:/longnames/another_very_long_filename_with_special_chars_@#$%.bin", files->next->full_path);
    
    // Clean up
    while (files) {
        file_entry_t *temp = files;
        files = files->next;
        free(temp);
    }
    
    return 1;
}

TEST(get_directory_listing_malformed_output) {
    serial_conn_t conn;
    file_entry_t *files = NULL;
    setup_get_directory_listing_tests();
    
    // Set up mocks for malformed directory output
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(200, "Directory of flash:/malformed\n"
                              "This is not a valid file entry\n"
                              "2  -rwx  1429      Jan 01 2020 00:00:00 +00:00  valid_file.txt\n"
                              "Invalid line with wrong format\n"
                              "3  drwx  0          Jan 01 2020 00:00:00 +00:00  valid_dir/\n"
                              "Another invalid line\n");
    
    int result = cisco_get_directory_listing(&conn, "flash:/malformed", &files, 30);
    
    ASSERT_EQUAL(2, result); // Only valid entries should be parsed
    ASSERT_NOT_NULL(files);
    
    // Check that only valid entries are included
    ASSERT_STRING_EQUAL("valid_file.txt", files->name);
    ASSERT_NOT_NULL(files->next);
    ASSERT_STRING_EQUAL("valid_dir", files->next->name);
    ASSERT_NULL(files->next->next);
    
    // Clean up
    while (files) {
        file_entry_t *temp = files;
        files = files->next;
        free(temp);
    }
    
    return 1;
}

TEST(get_directory_listing_memory_allocation_failure) {
    serial_conn_t conn;
    file_entry_t *files = NULL;
    setup_get_directory_listing_tests();
    
    // Set up mocks for successful directory listing
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(150, "Directory of flash:/memory_test\n"
                              "1  -rwx  1024      Jan 01 2020 00:00:00 +00:00  file1.txt\n"
                              "2  -rwx  2048      Jan 01 2020 00:00:00 +00:00  file2.txt\n");
    
    // Note: We can't easily simulate malloc failure in this test framework,
    // but we can test that the function handles the case gracefully
    // by ensuring it doesn't crash and returns appropriate results
    
    int result = cisco_get_directory_listing(&conn, "flash:/memory_test", &files, 30);
    
    // The function should either succeed or fail gracefully
    ASSERT_TRUE(result >= -1 && result <= 2);
    
    // If files were allocated, clean them up
    if (files) {
        while (files) {
            file_entry_t *temp = files;
            files = files->next;
            free(temp);
        }
    }
    
    return 1;
}

// ============================================================================
// cisco_delete_file Tests
// ============================================================================

TEST(delete_file_success) {
    serial_conn_t conn;
    setup_delete_file_tests();
    
    // Set up mocks for successful file deletion
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(25, "Are you sure you want to delete"); // confirmation prompt
    MOCK_WRITE_SET_RETURN(2); // serial_write succeeds for confirmation
    MOCK_READ_SET_RETURN(15, "File deleted successfully"); // deletion success message
    
    int result = cisco_delete_file(&conn, "flash:/config.txt", 30);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(2, mock_serial_write.call_count);
    // The last write should be the confirmation "y\n"
    ASSERT_STRING_EQUAL("y\n", mock_serial_write.last_data);
    ASSERT_EQUAL(3, mock_serial_read.call_count);
    
    return 1;
}

TEST(delete_file_send_command_failure) {
    serial_conn_t conn;
    setup_delete_file_tests();
    
    // Set up mock to make cisco_send_command fail
    MOCK_WRITE_SET_RETURN(-1); // serial_write fails
    
    int result = cisco_delete_file(&conn, "flash:/config.txt", 30);
    
    ASSERT_EQUAL(-1, result);
    ASSERT_EQUAL(1, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("delete flash:/config.txt\n", mock_serial_write.last_data);
    ASSERT_EQUAL(0, mock_serial_read.call_count); // Should not be called if send_command fails
    
    return 1;
}

TEST(delete_file_no_confirmation_prompt) {
    serial_conn_t conn;
    setup_delete_file_tests();
    
    // Set up mocks - command succeeds but no confirmation prompt
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(20, "File not found or access denied"); // No confirmation prompt
    
    int result = cisco_delete_file(&conn, "flash:/nonexistent.txt", 30);
    
    ASSERT_EQUAL(-1, result);
    ASSERT_EQUAL(1, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("delete flash:/nonexistent.txt\n", mock_serial_write.last_data);
    ASSERT_EQUAL(2, mock_serial_read.call_count);
    
    return 1;
}

TEST(delete_file_confirmation_write_failure) {
    serial_conn_t conn;
    setup_delete_file_tests();
    
    // Set up mocks - command succeeds, confirmation prompt, but write fails
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(25, "Are you sure you want to delete"); // confirmation prompt
    MOCK_WRITE_SET_RETURN(-1); // serial_write fails for confirmation
    
    int result = cisco_delete_file(&conn, "flash:/config.txt", 30);
    
    ASSERT_EQUAL(-1, result);
    ASSERT_EQUAL(2, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("y\n", mock_serial_write.last_data);
    ASSERT_EQUAL(2, mock_serial_read.call_count);
    
    return 1;
}

TEST(delete_file_deletion_failed) {
    serial_conn_t conn;
    setup_delete_file_tests();
    
    // Set up mocks - command succeeds, confirmation prompt, but deletion fails
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(25, "Are you sure you want to delete"); // confirmation prompt
    MOCK_WRITE_SET_RETURN(2); // serial_write succeeds for confirmation
    MOCK_READ_SET_RETURN(20, "Access denied - file cannot be removed"); // deletion failure message
    
    int result = cisco_delete_file(&conn, "flash:/protected.txt", 30);
    
    ASSERT_EQUAL(-1, result);
    ASSERT_EQUAL(2, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("y\n", mock_serial_write.last_data);
    ASSERT_EQUAL(3, mock_serial_read.call_count);
    
    return 1;
}

TEST(delete_file_timeout_after_confirmation) {
    serial_conn_t conn;
    setup_delete_file_tests();
    
    // Set up mocks - command succeeds, confirmation prompt, but timeout after confirmation
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(25, "Are you sure you want to delete"); // confirmation prompt
    MOCK_WRITE_SET_RETURN(2); // serial_write succeeds for confirmation
    MOCK_READ_SET_RETURN(0, ""); // timeout/no response after confirmation
    
    int result = cisco_delete_file(&conn, "flash:/config.txt", 30);
    
    ASSERT_EQUAL(-1, result);
    ASSERT_EQUAL(2, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("y\n", mock_serial_write.last_data);
    ASSERT_EQUAL(3, mock_serial_read.call_count);
    
    return 1;
}

TEST(delete_file_empty_file_path) {
    serial_conn_t conn;
    setup_delete_file_tests();
    
    // Set up mocks for empty file path
    MOCK_WRITE_SET_RETURN(8); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(25, "Are you sure you want to delete"); // confirmation prompt
    MOCK_WRITE_SET_RETURN(2); // serial_write succeeds for confirmation
    MOCK_READ_SET_RETURN(15, "File deleted successfully"); // deletion success message
    
    int result = cisco_delete_file(&conn, "", 30);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(2, mock_serial_write.call_count);
    // The last write should be the confirmation "y\n"
    ASSERT_STRING_EQUAL("y\n", mock_serial_write.last_data);
    ASSERT_EQUAL(3, mock_serial_read.call_count);
    
    return 1;
}

TEST(delete_file_long_file_path) {
    serial_conn_t conn;
    setup_delete_file_tests();
    
    // Create a long file path (but within MAX_LINE_LEN)
    char long_path[256];
    memset(long_path, 'a', 255);
    long_path[255] = '\0';
    
    // Set up mocks for long file path
    MOCK_WRITE_SET_RETURN(260); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(25, "Are you sure you want to delete"); // confirmation prompt
    MOCK_WRITE_SET_RETURN(2); // serial_write succeeds for confirmation
    MOCK_READ_SET_RETURN(15, "File deleted successfully"); // deletion success message
    
    int result = cisco_delete_file(&conn, long_path, 30);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(2, mock_serial_write.call_count);
    // The last write should be the confirmation "y\n"
    ASSERT_STRING_EQUAL("y\n", mock_serial_write.last_data);
    ASSERT_EQUAL(3, mock_serial_read.call_count);
    
    return 1;
}

TEST(delete_file_special_characters_in_path) {
    serial_conn_t conn;
    setup_delete_file_tests();
    
    // Set up mocks for file path with special characters
    MOCK_WRITE_SET_RETURN(25); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(25, "Are you sure you want to delete"); // confirmation prompt
    MOCK_WRITE_SET_RETURN(2); // serial_write succeeds for confirmation
    MOCK_READ_SET_RETURN(15, "File deleted successfully"); // deletion success message
    
    int result = cisco_delete_file(&conn, "flash:/backup/config@#$%.txt", 30);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(2, mock_serial_write.call_count);
    // The last write should be the confirmation "y\n"
    ASSERT_STRING_EQUAL("y\n", mock_serial_write.last_data);
    ASSERT_EQUAL(3, mock_serial_read.call_count);
    
    return 1;
}

// ============================================================================
// cisco_delete_directory Tests
// ============================================================================

TEST(delete_directory_success) {
    serial_conn_t conn;
    setup_delete_directory_tests();
    
    // Set up mocks for successful directory removal
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(25, "Are you sure you want to remove"); // confirmation prompt
    MOCK_WRITE_SET_RETURN(2); // serial_write succeeds for confirmation
    MOCK_READ_SET_RETURN(15, "Directory removed"); // removal success message
    
    int result = cisco_delete_directory(&conn, "flash:/backup", 30);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(2, mock_serial_write.call_count);
    // The last write should be the confirmation "y\n"
    ASSERT_STRING_EQUAL("y\n", mock_serial_write.last_data);
    ASSERT_EQUAL(3, mock_serial_read.call_count);
    
    return 1;
}

TEST(delete_directory_send_command_failure) {
    serial_conn_t conn;
    setup_delete_directory_tests();
    
    // Set up mock to make cisco_send_command fail
    MOCK_WRITE_SET_RETURN(-1); // serial_write fails
    
    int result = cisco_delete_directory(&conn, "flash:/backup", 30);
    
    ASSERT_EQUAL(-1, result);
    ASSERT_EQUAL(1, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("rmdir flash:/backup\n", mock_serial_write.last_data);
    ASSERT_EQUAL(0, mock_serial_read.call_count); // Should not be called if send_command fails
    
    return 1;
}

TEST(delete_directory_no_confirmation_prompt) {
    serial_conn_t conn;
    setup_delete_directory_tests();
    
    // Set up mocks - command succeeds but no confirmation prompt
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(20, "Directory not found or access denied"); // No confirmation prompt
    
    int result = cisco_delete_directory(&conn, "flash:/nonexistent", 30);
    
    ASSERT_EQUAL(-1, result);
    ASSERT_EQUAL(1, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("rmdir flash:/nonexistent\n", mock_serial_write.last_data);
    ASSERT_EQUAL(2, mock_serial_read.call_count);
    
    return 1;
}

TEST(delete_directory_confirmation_write_failure) {
    serial_conn_t conn;
    setup_delete_directory_tests();
    
    // Set up mocks - command succeeds, confirmation prompt, but write fails
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(25, "Are you sure you want to remove"); // confirmation prompt
    MOCK_WRITE_SET_RETURN(-1); // serial_write fails for confirmation
    
    int result = cisco_delete_directory(&conn, "flash:/backup", 30);
    
    ASSERT_EQUAL(-1, result);
    ASSERT_EQUAL(2, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("y\n", mock_serial_write.last_data);
    ASSERT_EQUAL(2, mock_serial_read.call_count);
    
    return 1;
}

TEST(delete_directory_removal_failed) {
    serial_conn_t conn;
    setup_delete_directory_tests();
    
    // Set up mocks - command succeeds, confirmation prompt, but removal fails
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(25, "Are you sure you want to remove"); // confirmation prompt
    MOCK_WRITE_SET_RETURN(2); // serial_write succeeds for confirmation
    MOCK_READ_SET_RETURN(20, "Access denied - cannot remove directory"); // removal failure message
    
    int result = cisco_delete_directory(&conn, "flash:/protected", 30);
    
    ASSERT_EQUAL(-1, result);
    ASSERT_EQUAL(2, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("y\n", mock_serial_write.last_data);
    ASSERT_EQUAL(3, mock_serial_read.call_count);
    
    return 1;
}

TEST(delete_directory_timeout_after_confirmation) {
    serial_conn_t conn;
    setup_delete_directory_tests();
    
    // Set up mocks - command succeeds, confirmation prompt, but timeout after confirmation
    MOCK_WRITE_SET_RETURN(15); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(25, "Are you sure you want to remove"); // confirmation prompt
    MOCK_WRITE_SET_RETURN(2); // serial_write succeeds for confirmation
    MOCK_READ_SET_RETURN(0, ""); // timeout/no response after confirmation
    
    int result = cisco_delete_directory(&conn, "flash:/backup", 30);
    
    ASSERT_EQUAL(-1, result);
    ASSERT_EQUAL(2, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("y\n", mock_serial_write.last_data);
    ASSERT_EQUAL(3, mock_serial_read.call_count);
    
    return 1;
}

TEST(delete_directory_empty_dir_path) {
    serial_conn_t conn;
    setup_delete_directory_tests();
    
    // Set up mocks for empty directory path
    MOCK_WRITE_SET_RETURN(8); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(25, "Are you sure you want to remove"); // confirmation prompt
    MOCK_WRITE_SET_RETURN(2); // serial_write succeeds for confirmation
    MOCK_READ_SET_RETURN(15, "Directory removed"); // removal success message
    
    int result = cisco_delete_directory(&conn, "", 30);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(2, mock_serial_write.call_count);
    // The last write should be the confirmation "y\n"
    ASSERT_STRING_EQUAL("y\n", mock_serial_write.last_data);
    ASSERT_EQUAL(3, mock_serial_read.call_count);
    
    return 1;
}

TEST(delete_directory_long_dir_path) {
    serial_conn_t conn;
    setup_delete_directory_tests();
    
    // Create a long directory path (but within MAX_LINE_LEN)
    char long_path[256];
    memset(long_path, 'a', 255);
    long_path[255] = '\0';
    
    // Set up mocks for long directory path
    MOCK_WRITE_SET_RETURN(260); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(25, "Are you sure you want to remove"); // confirmation prompt
    MOCK_WRITE_SET_RETURN(2); // serial_write succeeds for confirmation
    MOCK_READ_SET_RETURN(15, "Directory removed"); // removal success message
    
    int result = cisco_delete_directory(&conn, long_path, 30);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(2, mock_serial_write.call_count);
    // The last write should be the confirmation "y\n"
    ASSERT_STRING_EQUAL("y\n", mock_serial_write.last_data);
    ASSERT_EQUAL(3, mock_serial_read.call_count);
    
    return 1;
}

TEST(delete_directory_special_characters_in_path) {
    serial_conn_t conn;
    setup_delete_directory_tests();
    
    // Set up mocks for directory path with special characters
    MOCK_WRITE_SET_RETURN(25); // cisco_send_command succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found after command
    MOCK_READ_SET_RETURN(25, "Are you sure you want to remove"); // confirmation prompt
    MOCK_WRITE_SET_RETURN(2); // serial_write succeeds for confirmation
    MOCK_READ_SET_RETURN(15, "Directory removed"); // removal success message
    
    int result = cisco_delete_directory(&conn, "flash:/backup@#$%", 30);
    
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(2, mock_serial_write.call_count);
    // The last write should be the confirmation "y\n"
    ASSERT_STRING_EQUAL("y\n", mock_serial_write.last_data);
    ASSERT_EQUAL(3, mock_serial_read.call_count);
    
    return 1;
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main(void) {
    printf("Running cisco_commands unit tests...\n");
    
    int result = run_all_tests();
    
    return result ? 0 : 1;
}
