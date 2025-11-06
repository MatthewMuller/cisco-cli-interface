/**
 * @file test_framework.h
 * @brief Streamlined C test framework for the Cisco CLI interface project
 *
 * This header provides a modern testing framework with:
 * - Simple test registration and discovery
 * - Test fixtures for setup/teardown
 * - Test suites for organization
 * - Simplified mock framework
 * - Better assertion macros
 *
 * @author Cisco CLI Interface Team
 * @version 1.0
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// Test Discovery and Registration
// ============================================================================

/**
 * @brief Test function signature
 */
typedef int (*test_func_t)(void);

/**
 * @brief Test fixture function signatures
 */
typedef void (*test_setup_func_t)(void);
typedef void (*test_teardown_func_t)(void);

/**
 * @brief Test suite structure
 */
typedef struct test_suite {
  const char *name;
  test_setup_func_t setup;
  test_teardown_func_t teardown;
  test_func_t *tests;
  int test_count;
} test_suite_t;

/**
 * @brief Define a test function
 *
 * Usage: TEST(test_name) { ... }
 */
#define TEST(name) int test_##name(void)

/**
 * @brief Register a test function
 *
 * Usage: TEST_REGISTER(test_function_name)
 */
#define TEST_REGISTER(name) test_func_t test_##name##_ptr = test_##name

/**
 * @brief Define a test suite
 */
#define TEST_SUITE(name, setup_func, teardown_func)                            \
  test_suite_t test_suite_##name = {.name = #name,                             \
                                    .setup = setup_func,                       \
                                    .teardown = teardown_func,                 \
                                    .tests = NULL,                             \
                                    .test_count = 0}

// ============================================================================
// Assertion Macros
// ============================================================================

/**
 * @brief Assert that a condition is true
 */
#define ASSERT_TRUE(condition)                                                 \
  do {                                                                         \
    if (!(condition)) {                                                        \
      printf("FAIL: %s:%d - Assertion failed: %s\n", __FILE__, __LINE__,       \
             #condition);                                                      \
      return 0;                                                                \
    }                                                                          \
  } while (0)

/**
 * @brief Assert that a condition is false
 */
#define ASSERT_FALSE(condition)                                                \
  do {                                                                         \
    if ((condition)) {                                                         \
      printf("FAIL: %s:%d - Assertion failed: %s should be false\n", __FILE__, \
             __LINE__, #condition);                                            \
      return 0;                                                                \
    }                                                                          \
  } while (0)

/**
 * @brief Assert that two integers are equal
 */
#define ASSERT_EQUAL(expected, actual)                                         \
  do {                                                                         \
    if ((expected) != (actual)) {                                              \
      printf("FAIL: %s:%d - Expected %ld, got %ld\n", __FILE__, __LINE__,      \
             (long)(expected), (long)(actual));                                \
      return 0;                                                                \
    }                                                                          \
  } while (0)

/**
 * @brief Assert that two strings are equal
 */
#define ASSERT_STRING_EQUAL(expected, actual)                                  \
  do {                                                                         \
    if (strcmp((expected), (actual)) != 0) {                                   \
      printf("FAIL: %s:%d - Expected '%s', got '%s'\n", __FILE__, __LINE__,    \
             (expected), (actual));                                            \
      return 0;                                                                \
    }                                                                          \
  } while (0)

/**
 * @brief Assert that a pointer is not NULL
 */
#define ASSERT_NOT_NULL(ptr)                                                   \
  do {                                                                         \
    if ((ptr) == NULL) {                                                       \
      printf("FAIL: %s:%d - Pointer is NULL: %s\n", __FILE__, __LINE__, #ptr); \
      return 0;                                                                \
    }                                                                          \
  } while (0)

/**
 * @brief Assert that a pointer is NULL
 */
#define ASSERT_NULL(ptr)                                                       \
  do {                                                                         \
    if ((ptr) != NULL) {                                                       \
      printf("FAIL: %s:%d - Pointer is not NULL: %s\n", __FILE__, __LINE__,    \
             #ptr);                                                            \
      return 0;                                                                \
    }                                                                          \
  } while (0)

// ============================================================================
// Mock Framework
// ============================================================================

/**
 * @brief Mock state structure for serial functions
 */
typedef struct {
  int call_count;
  int return_values[20];
  char buffer_values[20][1024];
  int return_index;
  char last_data[1024];
} mock_serial_t;

extern mock_serial_t mock_serial_read;
extern mock_serial_t mock_serial_write;

/**
 * @brief Initialize mock state
 */
#define MOCK_INIT(mock)                                                        \
  do {                                                                         \
    (mock).call_count = 0;                                                     \
    (mock).return_index = 0;                                                   \
    memset((mock).return_values, 0, sizeof((mock).return_values));             \
    memset((mock).buffer_values, 0, sizeof((mock).buffer_values));             \
    memset((mock).last_data, 0, sizeof((mock).last_data));                     \
  } while (0)

/**
 * @brief Set up mock return values
 */
#define MOCK_SET_RETURN(mock, return_val, buffer_content)                      \
  do {                                                                         \
    if ((mock).return_index < 20) {                                            \
      (mock).return_values[(mock).return_index] = (return_val);                \
      if ((buffer_content) != NULL) {                                          \
        strncpy((mock).buffer_values[(mock).return_index], (buffer_content),   \
                1023);                                                         \
      } else {                                                                 \
        (mock).buffer_values[(mock).return_index][0] = '\0';                   \
      }                                                                        \
      (mock).return_index++;                                                   \
    }                                                                          \
  } while (0)

/**
 * @brief Set up mock return values (read function)
 */
#define MOCK_READ_SET_RETURN(return_val, buffer_content)                       \
  MOCK_SET_RETURN(mock_serial_read, return_val, buffer_content)

/**
 * @brief Set up mock return values (write function)
 */
#define MOCK_WRITE_SET_RETURN(return_val)                                      \
  MOCK_SET_RETURN(mock_serial_write, return_val, "")

/**
 * @brief Initialize all mocks
 */
#define MOCK_INIT_ALL()                                                        \
  do {                                                                         \
    MOCK_INIT(mock_serial_read);                                               \
    MOCK_INIT(mock_serial_write);                                              \
  } while (0)

// ============================================================================
// Test Runner Functions
// ============================================================================

/**
 * @brief Run all registered tests
 */
int run_all_tests(void);

/**
 * @brief Run tests in a specific suite
 */
int run_test_suite(const char *suite_name);

/**
 * @brief Get test statistics
 */
void print_test_stats(void);

// ============================================================================
// Test Registry Declarations
// ============================================================================

/**
 * @brief Test registry - array of test function pointers
 */
extern test_func_t test_registry[];

/**
 * @brief Number of tests in registry
 */
extern int test_registry_count;

/**
 * @brief Test names registry - parallel array to test_registry
 */
extern const char *test_names[];

/**
 * @brief Number of test names in registry
 */
extern int test_names_count;

// ============================================================================
// Test Fixture Helpers
// ============================================================================

/**
 * @brief Define a test setup function
 */
#define TEST_SETUP(name) void test_setup_##name(void)

/**
 * @brief Define a test teardown function
 */
#define TEST_TEARDOWN(name) void test_teardown_##name(void)

/**
 * @brief Common test setup for serial tests
 */
TEST_SETUP(serial);

/**
 * @brief Common test teardown for serial tests
 */
TEST_TEARDOWN(serial);

// ============================================================================
// Output Suppression Functions
// ============================================================================

/**
 * @brief Suppress stdout output during test execution
 */
void suppress_stdout(void);

/**
 * @brief Restore stdout output after test execution
 */
void restore_stdout(void);

#endif // TEST_FRAMEWORK_H
