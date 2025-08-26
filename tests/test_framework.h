#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Simple assertion macros
#define TEST_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            printf("FAIL: %s:%d - Assertion failed: %s\n", __FILE__, __LINE__, #condition); \
            return 0; \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            printf("FAIL: %s:%d - Expected %d, got %d\n", __FILE__, __LINE__, (expected), (actual)); \
            return 0; \
        } \
    } while(0)

#define TEST_ASSERT_STRING_EQUAL(expected, actual) \
    do { \
        if (strcmp((expected), (actual)) != 0) { \
            printf("FAIL: %s:%d - Expected '%s', got '%s'\n", __FILE__, __LINE__, (expected), (actual)); \
            return 0; \
        } \
    } while(0)

// Test function type
typedef int (*test_function_t)(void);

// Test runner declaration
int run_test(const char *test_name, test_function_t test_func);

// Mock framework
typedef struct {
    int call_count;
    int return_values[10];
    char buffer_values[10][1024];
    int return_index;
} mock_serial_read_until_t;

extern mock_serial_read_until_t mock_serial_read_until;

// Mock function declaration
int mock_serial_read_until_func(void *conn, char *buffer, int max_len, const char *delimiter);

// Mock setup functions
void mock_serial_read_until_init(void);
void mock_serial_read_until_set_return(int return_value, const char *buffer_content);

#endif // TEST_FRAMEWORK_H
