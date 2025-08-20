#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include "cisco_cli.h"

// Test framework colors
#define TEST_COLOR_RED     "\033[31m"
#define TEST_COLOR_GREEN   "\033[32m"
#define TEST_COLOR_YELLOW  "\033[33m"
#define TEST_COLOR_BLUE    "\033[34m"
#define TEST_COLOR_RESET   "\033[0m"

// Test result tracking
typedef struct {
    int total_tests;
    int passed_tests;
    int failed_tests;
    char current_test_name[256];
} test_context_t;

extern test_context_t test_ctx;

// Test macros
#define TEST_START(name) do { \
    strncpy(test_ctx.current_test_name, name, sizeof(test_ctx.current_test_name) - 1); \
    test_ctx.current_test_name[sizeof(test_ctx.current_test_name) - 1] = '\0'; \
    printf("%s[TEST]%s %s\n", TEST_COLOR_BLUE, TEST_COLOR_RESET, name); \
} while(0)

#define TEST_ASSERT(condition) do { \
    if (!(condition)) { \
        printf("%s[FAIL]%s %s: Assertion failed: %s\n", \
               TEST_COLOR_RED, TEST_COLOR_RESET, \
               test_ctx.current_test_name, #condition); \
        test_ctx.failed_tests++; \
        return; \
    } \
} while(0)

#define TEST_ASSERT_EQ(expected, actual) do { \
    if ((expected) != (actual)) { \
        printf("%s[FAIL]%s %s: Expected %d, got %d\n", \
               TEST_COLOR_RED, TEST_COLOR_RESET, \
               test_ctx.current_test_name, (int)(expected), (int)(actual)); \
        test_ctx.failed_tests++; \
        return; \
    } \
} while(0)

#define TEST_ASSERT_STR_EQ(expected, actual) do { \
    if (strcmp((expected), (actual)) != 0) { \
        printf("%s[FAIL]%s %s: Expected '%s', got '%s'\n", \
               TEST_COLOR_RED, TEST_COLOR_RESET, \
               test_ctx.current_test_name, (expected), (actual)); \
        test_ctx.failed_tests++; \
        return; \
    } \
} while(0)

#define TEST_ASSERT_NULL(ptr) do { \
    if ((ptr) != NULL) { \
        printf("%s[FAIL]%s %s: Expected NULL, got non-NULL\n", \
               TEST_COLOR_RED, TEST_COLOR_RESET, \
               test_ctx.current_test_name); \
        test_ctx.failed_tests++; \
        return; \
    } \
} while(0)

#define TEST_ASSERT_NOT_NULL(ptr) do { \
    if ((ptr) == NULL) { \
        printf("%s[FAIL]%s %s: Expected non-NULL, got NULL\n", \
               TEST_COLOR_RED, TEST_COLOR_RESET, \
               test_ctx.current_test_name); \
        test_ctx.failed_tests++; \
        return; \
    } \
} while(0)

#define TEST_PASS() do { \
    printf("%s[PASS]%s %s\n", TEST_COLOR_GREEN, TEST_COLOR_RESET, \
           test_ctx.current_test_name); \
    test_ctx.passed_tests++; \
} while(0)

// Test suite macros
#define TEST_SUITE_START(name) do { \
    printf("\n%s=== Test Suite: %s ===%s\n", \
           TEST_COLOR_YELLOW, name, TEST_COLOR_RESET); \
} while(0)

#define TEST_SUITE_END() do { \
    printf("\n%s=== Test Results ===%s\n", \
           TEST_COLOR_YELLOW, TEST_COLOR_RESET); \
    printf("Total: %d, Passed: %d, Failed: %d\n", \
           test_ctx.total_tests, test_ctx.passed_tests, test_ctx.failed_tests); \
    if (test_ctx.failed_tests == 0) { \
        printf("%sAll tests passed!%s\n", TEST_COLOR_GREEN, TEST_COLOR_RESET); \
    } else { \
        printf("%s%d tests failed!%s\n", \
               TEST_COLOR_RED, test_ctx.failed_tests, TEST_COLOR_RESET); \
    } \
} while(0)

// Mock framework for serial communication
typedef struct {
    char *read_data;           // Data to return on read
    int read_data_len;         // Length of read data
    int read_data_pos;         // Current position in read data
    char expected_write[1024]; // Expected data to be written
    int write_called;          // Number of times write was called
    char last_written[1024];   // Last data written
    int return_value;          // Return value for operations
    int timeout_occurred;      // Whether timeout should occur
} mock_serial_t;

extern mock_serial_t mock_serial;

// Mock function declarations
void mock_serial_init(void);
void mock_serial_reset(void);
void mock_serial_set_read_data(const char *data);
void mock_serial_set_expected_write(const char *data);
void mock_serial_set_return_value(int value);
void mock_serial_set_timeout(int timeout);

// Mock implementations of serial functions
int mock_serial_write(serial_conn_t *conn, const char *data);
int mock_serial_read(serial_conn_t *conn, char *buffer, int max_len);
int mock_serial_read_until(serial_conn_t *conn, char *buffer, int max_len, const char *delimiter);

// Test runner
void run_all_tests(void);

// Individual test function declarations
void test_cisco_wait_for_prompt_success(void);
void test_cisco_wait_for_prompt_timeout(void);
void test_cisco_send_command_success(void);
void test_cisco_send_command_failure(void);
void test_cisco_init_flash_success(void);
void test_cisco_init_flash_timeout(void);
void test_cisco_get_directory_listing_success(void);
void test_cisco_get_directory_listing_empty(void);
void test_cisco_delete_file_success(void);
void test_cisco_delete_file_failure(void);
void test_cisco_delete_directory_success(void);
void test_cisco_delete_directory_failure(void);

#endif // TEST_FRAMEWORK_H
