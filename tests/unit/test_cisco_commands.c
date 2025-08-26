#include "../framework/test_framework.h"
#include "../include/cisco_cli.h"

// Mock the serial_read_until function by defining it here
// This will override the real function when linking
int serial_read_until(serial_conn_t *conn, char *buffer, int max_len, const char *delimiter) {
    return mock_serial_read_until_func(conn, buffer, max_len, delimiter);
}

// Mock state for serial_write function
typedef struct {
    int call_count;
    int return_values[10];
    char last_data[1024];
    int return_index;
} mock_serial_write_t;

mock_serial_write_t mock_serial_write;

void mock_serial_write_init(void) {
    mock_serial_write.call_count = 0;
    mock_serial_write.return_index = 0;
    memset(mock_serial_write.return_values, 0, sizeof(mock_serial_write.return_values));
    memset(mock_serial_write.last_data, 0, sizeof(mock_serial_write.last_data));
}

void mock_serial_write_set_return(int return_value) {
    if (mock_serial_write.return_index < 10) {
        mock_serial_write.return_values[mock_serial_write.return_index] = return_value;
        mock_serial_write.return_index++;
    }
}

// Mock other required functions
int serial_write(serial_conn_t *conn, const char *data) {
    (void)conn;
    mock_serial_write.call_count++;
    
    if (data) {
        strncpy(mock_serial_write.last_data, data, sizeof(mock_serial_write.last_data) - 1);
        mock_serial_write.last_data[sizeof(mock_serial_write.last_data) - 1] = '\0';
    }
    
    if (mock_serial_write.call_count <= mock_serial_write.return_index) {
        int index = mock_serial_write.call_count - 1;
        return mock_serial_write.return_values[index];
    }
    
    return 0; // Default return value
}

void trim_whitespace(char *str) {
    (void)str;
    // Simple implementation for testing
}

int is_binary_file(const char *filename) {
    (void)filename;
    return 0; // Assume not binary for testing
}

// Test: cisco_wait_for_prompt should return 0 when prompt is found immediately
int test_cisco_wait_for_prompt_success_immediate(void) {
    serial_conn_t conn;
    mock_serial_read_until_init();
    
    // Set up mock to return a line with prompt immediately
    mock_serial_read_until_set_return(15, "Router# : ");
    
    int result = cisco_wait_for_prompt(&conn, 1);
    
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(1, mock_serial_read_until.call_count);
    
    return 1;
}

// Test: cisco_wait_for_prompt should return 0 when prompt is found after some delay
int test_cisco_wait_for_prompt_success_delayed(void) {
    serial_conn_t conn;
    mock_serial_read_until_init();
    
    // Set up mock to return empty lines first, then prompt
    mock_serial_read_until_set_return(0, "");  // No data
    mock_serial_read_until_set_return(12, "Router# : ");  // Prompt found
    
    int result = cisco_wait_for_prompt(&conn, 2);
    
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(2, mock_serial_read_until.call_count);
    
    return 1;
}

// Test: cisco_wait_for_prompt should return -1 when timeout occurs
int test_cisco_wait_for_prompt_timeout(void) {
    serial_conn_t conn;
    mock_serial_read_until_init();
    
    // Set up mock to return no data (timeout scenario)
    // The function will call usleep and decrement timeout until it reaches 0
    // We don't need to set up multiple returns since the mock will return 0 by default
    
    int result = cisco_wait_for_prompt(&conn, 1);  // 1 second timeout
    
    TEST_ASSERT_EQUAL(-1, result);
    // Should have been called at least 1 time (once per timeout iteration)
    TEST_ASSERT(mock_serial_read_until.call_count >= 1);
    
    return 1;
}

// Test: cisco_wait_for_prompt should return 0 when prompt is found in data without prompt
int test_cisco_wait_for_prompt_no_prompt_in_data(void) {
    serial_conn_t conn;
    mock_serial_read_until_init();
    
    // Set up mock to return data without prompt, then data with prompt
    mock_serial_read_until_set_return(20, "Some output without prompt\n");
    mock_serial_read_until_set_return(15, "Router# : ");
    
    int result = cisco_wait_for_prompt(&conn, 2);
    
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(2, mock_serial_read_until.call_count);
    
    return 1;
}

// Test: cisco_wait_for_prompt should handle empty data correctly
int test_cisco_wait_for_prompt_empty_data(void) {
    serial_conn_t conn;
    mock_serial_read_until_init();
    
    // Set up mock to return empty data, then prompt
    mock_serial_read_until_set_return(0, "");
    mock_serial_read_until_set_return(0, "");
    mock_serial_read_until_set_return(10, "Router# : ");
    
    int result = cisco_wait_for_prompt(&conn, 3);
    
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(3, mock_serial_read_until.call_count);
    
    return 1;
}

// Test: cisco_send_command should format command with newline and send successfully
int test_cisco_send_command_success(void) {
    serial_conn_t conn;
    mock_serial_write_init();
    mock_serial_read_until_init();
    
    // Set up mocks
    mock_serial_write_set_return(10); // serial_write succeeds
    mock_serial_read_until_set_return(15, "Router# : "); // prompt found immediately
    
    int result = cisco_send_command(&conn, "show version", 5);
    
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(1, mock_serial_write.call_count);
    TEST_ASSERT_STRING_EQUAL("show version\n", mock_serial_write.last_data);
    TEST_ASSERT_EQUAL(1, mock_serial_read_until.call_count);
    
    return 1;
}

// Test: cisco_send_command should return -1 when serial_write fails
int test_cisco_send_command_serial_write_failure(void) {
    serial_conn_t conn;
    mock_serial_write_init();
    mock_serial_read_until_init();
    
    // Set up mock to make serial_write fail
    mock_serial_write_set_return(-1); // serial_write fails
    
    int result = cisco_send_command(&conn, "show version", 5);
    
    TEST_ASSERT_EQUAL(-1, result);
    TEST_ASSERT_EQUAL(1, mock_serial_write.call_count);
    TEST_ASSERT_STRING_EQUAL("show version\n", mock_serial_write.last_data);
    TEST_ASSERT_EQUAL(0, mock_serial_read_until.call_count); // Should not be called if write fails
    
    return 1;
}

// Test: cisco_send_command should return -1 when cisco_wait_for_prompt times out
int test_cisco_send_command_prompt_timeout(void) {
    serial_conn_t conn;
    mock_serial_write_init();
    mock_serial_read_until_init();
    
    // Set up mocks
    mock_serial_write_set_return(10); // serial_write succeeds
    // No mock setup for serial_read_until, so it will return 0 (timeout)
    
    int result = cisco_send_command(&conn, "show interfaces", 1);
    
    TEST_ASSERT_EQUAL(-1, result);
    TEST_ASSERT_EQUAL(1, mock_serial_write.call_count);
    TEST_ASSERT_STRING_EQUAL("show interfaces\n", mock_serial_write.last_data);
    TEST_ASSERT(mock_serial_read_until.call_count >= 1); // Should be called at least once
    
    return 1;
}

// Test: cisco_send_command should handle empty command correctly
int test_cisco_send_command_empty_command(void) {
    serial_conn_t conn;
    mock_serial_write_init();
    mock_serial_read_until_init();
    
    // Set up mocks
    mock_serial_write_set_return(1); // serial_write succeeds
    mock_serial_read_until_set_return(10, "Router# : "); // prompt found
    
    int result = cisco_send_command(&conn, "", 5);
    
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(1, mock_serial_write.call_count);
    TEST_ASSERT_STRING_EQUAL("\n", mock_serial_write.last_data);
    TEST_ASSERT_EQUAL(1, mock_serial_read_until.call_count);
    
    return 1;
}

// Test: cisco_send_command should handle long command correctly
int test_cisco_send_command_long_command(void) {
    serial_conn_t conn;
    mock_serial_write_init();
    mock_serial_read_until_init();
    
    // Create a long command (but within MAX_LINE_LEN)
    char long_command[512];
    memset(long_command, 'a', 511);
    long_command[511] = '\0';
    
    // Set up mocks
    mock_serial_write_set_return(512); // serial_write succeeds
    mock_serial_read_until_set_return(15, "Router# : "); // prompt found
    
    int result = cisco_send_command(&conn, long_command, 5);
    
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(1, mock_serial_write.call_count);
    // Check that the command was formatted with newline
    TEST_ASSERT(strlen(mock_serial_write.last_data) == strlen(long_command) + 1);
    TEST_ASSERT(mock_serial_write.last_data[strlen(long_command)] == '\n');
    TEST_ASSERT_EQUAL(1, mock_serial_read_until.call_count);
    
    return 1;
}

// Test: cisco_send_command should handle command with special characters
int test_cisco_send_command_special_characters(void) {
    serial_conn_t conn;
    mock_serial_write_init();
    mock_serial_read_until_init();
    
    // Set up mocks
    mock_serial_write_set_return(20); // serial_write succeeds
    mock_serial_read_until_set_return(15, "Router# : "); // prompt found
    
    int result = cisco_send_command(&conn, "show ip route | include 192.168.1.0/24", 5);
    
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(1, mock_serial_write.call_count);
    TEST_ASSERT_STRING_EQUAL("show ip route | include 192.168.1.0/24\n", mock_serial_write.last_data);
    TEST_ASSERT_EQUAL(1, mock_serial_read_until.call_count);
    
    return 1;
}

// Main test runner
int main(void) {
    printf("Running cisco_commands unit tests...\n\n");
    
    int passed = 0;
    int total = 0;
    
    // Test cisco_wait_for_prompt function
    printf("=== Testing cisco_wait_for_prompt ===\n");
    
    total++;
    if (run_test("cisco_wait_for_prompt_success_immediate", test_cisco_wait_for_prompt_success_immediate)) {
        passed++;
    }
    
    total++;
    if (run_test("cisco_wait_for_prompt_success_delayed", test_cisco_wait_for_prompt_success_delayed)) {
        passed++;
    }
    
    total++;
    if (run_test("cisco_wait_for_prompt_timeout", test_cisco_wait_for_prompt_timeout)) {
        passed++;
    }
    
    total++;
    if (run_test("cisco_wait_for_prompt_no_prompt_in_data", test_cisco_wait_for_prompt_no_prompt_in_data)) {
        passed++;
    }
    
    total++;
    if (run_test("cisco_wait_for_prompt_empty_data", test_cisco_wait_for_prompt_empty_data)) {
        passed++;
    }
    
    // Test cisco_send_command function
    printf("\n=== Testing cisco_send_command ===\n");
    
    total++;
    if (run_test("cisco_send_command_success", test_cisco_send_command_success)) {
        passed++;
    }
    
    total++;
    if (run_test("cisco_send_command_serial_write_failure", test_cisco_send_command_serial_write_failure)) {
        passed++;
    }
    
    total++;
    if (run_test("cisco_send_command_prompt_timeout", test_cisco_send_command_prompt_timeout)) {
        passed++;
    }
    
    total++;
    if (run_test("cisco_send_command_empty_command", test_cisco_send_command_empty_command)) {
        passed++;
    }
    
    total++;
    if (run_test("cisco_send_command_long_command", test_cisco_send_command_long_command)) {
        passed++;
    }
    
    total++;
    if (run_test("cisco_send_command_special_characters", test_cisco_send_command_special_characters)) {
        passed++;
    }
    
    printf("\nTest Summary: %d/%d tests passed\n", passed, total);
    
    return (passed == total) ? 0 : 1;
}
