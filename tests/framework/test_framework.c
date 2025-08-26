/**
 * @file test_framework.c
 * @brief Implementation of the simple C test framework
 * 
 * This file contains the implementation of:
 * - Mock framework for serial communication testing
 * - Test runner functionality
 * 
 * @author Cisco CLI Interface Team
 * @version 1.0
 */

#include "test_framework.h"

/** @brief Global mock state for serial_read_until function */
mock_serial_read_until_t mock_serial_read_until;

/**
 * @brief Initialize the mock serial_read_until state
 * 
 * Resets all mock state variables to their initial values:
 * - call_count: 0
 * - return_index: 0
 * - return_values: all zeros
 * - buffer_values: all empty strings
 * 
 * Call this function before each test that uses the mock to ensure
 * a clean state.
 */
void mock_serial_read_until_init(void) {
    mock_serial_read_until.call_count = 0;
    mock_serial_read_until.return_index = 0;
    memset(mock_serial_read_until.return_values, 0, sizeof(mock_serial_read_until.return_values));
    memset(mock_serial_read_until.buffer_values, 0, sizeof(mock_serial_read_until.buffer_values));
}

/**
 * @brief Set up a return value and buffer content for the mock function
 * 
 * Adds a return value and optional buffer content to the mock's predefined
 * responses. These will be returned on successive calls to the mock function
 * in the order they were set up.
 * 
 * The function supports up to 10 predefined responses. If more than 10
 * responses are set up, additional calls will be ignored.
 * 
 * @param return_value The return value to use (number of bytes "read")
 * @param buffer_content String to copy into the buffer (can be NULL for no content)
 */
void mock_serial_read_until_set_return(int return_value, const char *buffer_content) {
    if (mock_serial_read_until.return_index < 10) {
        mock_serial_read_until.return_values[mock_serial_read_until.return_index] = return_value;
        if (buffer_content) {
            strncpy(mock_serial_read_until.buffer_values[mock_serial_read_until.return_index], 
                   buffer_content, 1023);
        }
        mock_serial_read_until.return_index++;
    }
}

/**
 * @brief Mock implementation of serial_read_until function
 * 
 * This function replaces the real serial_read_until during testing.
 * It returns predefined values and copies predefined buffer contents
 * based on the mock state.
 * 
 * Behavior:
 * - Increments call_count on each call
 * - Returns the next predefined return value if available
 * - Copies the corresponding buffer content if return value > 0
 * - Returns 0 if no more predefined values are available
 * 
 * @param conn Connection handle (unused in mock implementation)
 * @param buffer Buffer to store read data (must be valid if return value > 0)
 * @param max_len Maximum length of buffer
 * @param delimiter Delimiter string (unused in mock implementation)
 * @return Number of bytes "read", or 0 if no more predefined values
 */
int mock_serial_read_until_func(void *conn, char *buffer, int max_len, const char *delimiter) {
    mock_serial_read_until.call_count++;
    
    if (mock_serial_read_until.call_count <= mock_serial_read_until.return_index) {
        int index = mock_serial_read_until.call_count - 1;
        int return_value = mock_serial_read_until.return_values[index];
        
        if (return_value > 0 && buffer) {
            strncpy(buffer, mock_serial_read_until.buffer_values[index], max_len - 1);
            buffer[max_len - 1] = '\0';
        }
        
        return return_value;
    }
    
    return 0;
}

/**
 * @brief Run a single test function
 * 
 * Executes the provided test function and reports the result to stdout.
 * The test function should return 1 for pass or 0 for fail.
 * 
 * Output format:
 * - "Running test: <test_name>"
 * - "PASS: <test_name>" or "FAIL: <test_name>"
 * 
 * @param test_name Human-readable name of the test for output
 * @param test_func Function pointer to the test function to execute
 * @return 1 if test passed, 0 if test failed
 */
int run_test(const char *test_name, test_function_t test_func) {
    printf("Running test: %s\n", test_name);
    int result = test_func();
    if (result) {
        printf("PASS: %s\n", test_name);
    } else {
        printf("FAIL: %s\n", test_name);
    }
    return result;
}
