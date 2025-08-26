#include "../framework/test_framework.h"
#include "../include/cisco_cli.h"

// Mock the serial_read_until function by defining it here
// This will override the real function when linking
int serial_read_until(serial_conn_t *conn, char *buffer, int max_len, const char *delimiter) {
    return mock_serial_read_until_func(conn, buffer, max_len, delimiter);
}

// Mock other required functions
int serial_write(serial_conn_t *conn, const char *data) {
    (void)conn;
    (void)data;
    return 0;
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
    
    int result = cisco_wait_for_prompt(&conn, 10);
    
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
    mock_serial_read_until_set_return(0, "");  // No data
    mock_serial_read_until_set_return(12, "Router# : ");  // Prompt found
    
    int result = cisco_wait_for_prompt(&conn, 10);
    
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(3, mock_serial_read_until.call_count);
    
    return 1;
}

// Test: cisco_wait_for_prompt should return -1 when timeout occurs
int test_cisco_wait_for_prompt_timeout(void) {
    serial_conn_t conn;
    mock_serial_read_until_init();
    
    // Set up mock to return no data (timeout scenario)
    // The function will call usleep and decrement timeout until it reaches 0
    // We don't need to set up multiple returns since the mock will return 0 by default
    
    int result = cisco_wait_for_prompt(&conn, 3);  // 3 iterations
    
    TEST_ASSERT_EQUAL(-1, result);
    // Should have been called at least 3 times (once per timeout iteration)
    TEST_ASSERT(mock_serial_read_until.call_count >= 3);
    
    return 1;
}

// Test: cisco_wait_for_prompt should return 0 when prompt is found in data without prompt
int test_cisco_wait_for_prompt_no_prompt_in_data(void) {
    serial_conn_t conn;
    mock_serial_read_until_init();
    
    // Set up mock to return data without prompt, then data with prompt
    mock_serial_read_until_set_return(20, "Some output without prompt\n");
    mock_serial_read_until_set_return(15, "Router# : ");
    
    int result = cisco_wait_for_prompt(&conn, 10);
    
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
    
    int result = cisco_wait_for_prompt(&conn, 10);
    
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(3, mock_serial_read_until.call_count);
    
    return 1;
}

// Main test runner
int main(void) {
    printf("Running cisco_wait_for_prompt unit tests...\n\n");
    
    int passed = 0;
    int total = 0;
    
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
    
    printf("\nTest Summary: %d/%d tests passed\n", passed, total);
    
    return (passed == total) ? 0 : 1;
}
