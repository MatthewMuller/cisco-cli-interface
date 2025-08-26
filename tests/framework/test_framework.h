/**
 * @file test_framework.h
 * @brief Simple C test framework for the Cisco CLI interface project
 * 
 * This header provides a lightweight testing framework with:
 * - Assertion macros for common test conditions
 * - Test runner functionality
 * - Mock framework for serial communication testing
 * 
 * @author Cisco CLI Interface Team
 * @version 1.0
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 * @brief Assert that a condition is true
 * 
 * If the condition is false, prints a failure message with file and line number,
 * then returns 0 to indicate test failure.
 * 
 * @param condition The boolean condition to test
 */
#define TEST_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            printf("FAIL: %s:%d - Assertion failed: %s\n", __FILE__, __LINE__, #condition); \
            return 0; \
        } \
    } while(0)

/**
 * @brief Assert that two integers are equal
 * 
 * Compares expected and actual values. If they don't match, prints a failure
 * message with the expected and actual values.
 * 
 * @param expected The expected integer value
 * @param actual The actual integer value to compare
 */
#define TEST_ASSERT_EQUAL(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            printf("FAIL: %s:%d - Expected %d, got %d\n", __FILE__, __LINE__, (expected), (actual)); \
            return 0; \
        } \
    } while(0)

/**
 * @brief Assert that two strings are equal
 * 
 * Compares expected and actual strings using strcmp. If they don't match,
 * prints a failure message with the expected and actual string values.
 * 
 * @param expected The expected string value
 * @param actual The actual string value to compare
 */
#define TEST_ASSERT_STRING_EQUAL(expected, actual) \
    do { \
        if (strcmp((expected), (actual)) != 0) { \
            printf("FAIL: %s:%d - Expected '%s', got '%s'\n", __FILE__, __LINE__, (expected), (actual)); \
            return 0; \
        } \
    } while(0)

/**
 * @brief Function pointer type for test functions
 * 
 * All test functions should return an integer:
 * - 1 (true) for test pass
 * - 0 (false) for test fail
 */
typedef int (*test_function_t)(void);

/**
 * @brief Run a single test function
 * 
 * Executes the provided test function and reports the result.
 * 
 * @param test_name Human-readable name of the test
 * @param test_func Function pointer to the test function
 * @return 1 if test passed, 0 if test failed
 */
int run_test(const char *test_name, test_function_t test_func);

/**
 * @brief Mock state structure for serial_read_until function
 * 
 * Tracks calls to the mocked serial_read_until function and provides
 * predefined return values and buffer contents for testing.
 */
typedef struct {
    int call_count;                    /**< Number of times the mock function was called */
    int return_values[10];             /**< Array of return values to return on successive calls */
    char buffer_values[10][1024];      /**< Array of buffer contents to copy on successive calls */
    int return_index;                  /**< Current index for setting up return values */
} mock_serial_read_until_t;

/** @brief Global mock state instance */
extern mock_serial_read_until_t mock_serial_read_until;

/**
 * @brief Mock implementation of serial_read_until function
 * 
 * This function replaces the real serial_read_until during testing.
 * It returns predefined values and copies predefined buffer contents
 * based on the mock state.
 * 
 * @param conn Connection handle (unused in mock)
 * @param buffer Buffer to store read data
 * @param max_len Maximum length of buffer
 * @param delimiter Delimiter string (unused in mock)
 * @return Number of bytes read, or 0 if no more predefined values
 */
int mock_serial_read_until_func(void *conn, char *buffer, int max_len, const char *delimiter);

/**
 * @brief Initialize the mock serial_read_until state
 * 
 * Resets all mock state variables to their initial values.
 * Call this before each test that uses the mock.
 */
void mock_serial_read_until_init(void);

/**
 * @brief Set up a return value and buffer content for the mock function
 * 
 * Adds a return value and optional buffer content to the mock's predefined
 * responses. These will be returned on successive calls to the mock function.
 * 
 * @param return_value The return value to use (number of bytes "read")
 * @param buffer_content String to copy into the buffer (can be NULL)
 */
void mock_serial_read_until_set_return(int return_value, const char *buffer_content);

#endif // TEST_FRAMEWORK_H
