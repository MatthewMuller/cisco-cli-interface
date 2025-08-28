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
    NULL
};

int test_registry_count = sizeof(test_registry) / sizeof(test_registry[0]) - 1;

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
// Main Test Runner
// ============================================================================

int main(void) {
    printf("Running cisco_commands unit tests (v2)...\n");
    
    int result = run_all_tests();
    
    return result ? 0 : 1;
}
