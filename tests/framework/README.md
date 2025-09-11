# Cisco CLI Interface Testing Framework

A streamlined, lightweight C testing framework designed specifically for the Cisco CLI interface project. This framework provides modern testing capabilities with minimal overhead and maximum simplicity.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Quick Start](#quick-start)
- [Writing Tests](#writing-tests)
- [Mock Framework](#mock-framework)
- [Assertions](#assertions)
- [Test Organization](#test-organization)
- [Build and Run](#build-and-run)
- [Best Practices](#best-practices)
- [Advanced Features](#advanced-features)
- [Troubleshooting](#troubleshooting)

## Overview

The Cisco CLI Interface Testing Framework is a custom-built testing solution that provides:

- **Simple test registration** - No complex setup required
- **Built-in mocking** - Mock serial communication functions easily
- **Comprehensive assertions** - Rich set of assertion macros
- **Test fixtures** - Setup and teardown support
- **Test organization** - Group tests into logical suites
- **Clean output** - Suppressed stdout during test execution
- **Statistics** - Detailed test run statistics

## Features

### Core Features
- ✅ **Test Discovery**: Automatic test registration and discovery
- ✅ **Mock Framework**: Built-in mocking for serial communication
- ✅ **Assertion Macros**: Comprehensive set of assertion helpers
- ✅ **Test Fixtures**: Setup and teardown functions
- ✅ **Test Suites**: Organize tests into logical groups
- ✅ **Output Suppression**: Clean test output without function printf noise
- ✅ **Statistics**: Detailed pass/fail statistics with success rates

### Mock Capabilities
- ✅ **Serial Communication Mocking**: Mock `serial_read_until` and `serial_write`
- ✅ **Return Value Control**: Set specific return values for mock calls
- ✅ **Buffer Content Control**: Control what data mocks return
- ✅ **Call Tracking**: Track how many times functions are called
- ✅ **Sequential Mocking**: Set up different responses for multiple calls

## Quick Start

### 1. Include the Framework

```c
#include "test_framework.h"
```

### 2. Write a Simple Test

```c
TEST(my_first_test) {
    // Your test code here
    ASSERT_TRUE(1 == 1);
    return 1; // Return 1 for pass, 0 for fail
}
```

### 3. Register Your Test

```c
// In your test file, add to the registry
test_func_t test_registry[] = {
    test_my_first_test,
    NULL
};

int test_registry_count = sizeof(test_registry) / sizeof(test_registry[0]) - 1;
```

### 4. Build and Run

```bash
cd tests
make test
```

## Writing Tests

### Basic Test Structure

```c
TEST(test_name) {
    // Setup (if needed)
    MOCK_INIT_ALL();

    // Test logic
    int result = function_under_test();

    // Assertions
    ASSERT_EQUAL(0, result);
    ASSERT_TRUE(some_condition);

    // Return 1 for pass, 0 for fail
    return 1;
}
```

### Test Function Signature

All test functions must follow this signature:
```c
int test_function_name(void);
```

- Return `1` for test pass
- Return `0` for test fail
- Use assertion macros for automatic failure handling

### Test Registration

Tests are registered in two arrays:

```c
// Function pointers
test_func_t test_registry[] = {
    test_function_name,
    test_another_function,
    NULL  // Always end with NULL
};

// Test names (parallel array)
const char* test_names[] = {
    "function_name",
    "another_function",
    NULL  // Always end with NULL
};

// Counts
int test_registry_count = sizeof(test_registry) / sizeof(test_registry[0]) - 1;
int test_names_count = sizeof(test_names) / sizeof(test_names[0]) - 1;
```

## Mock Framework

The framework includes a powerful mocking system for testing serial communication functions.

### Mock Initialization

```c
// Initialize all mocks
MOCK_INIT_ALL();

// Or initialize individual mocks
MOCK_INIT(mock_serial_read);
MOCK_INIT(mock_serial_write);
```

### Setting Up Mock Return Values

```c
// Set up serial_read_until mock
MOCK_READ_SET_RETURN(15, "Router# : ");  // Return 15 bytes with prompt

// Set up serial_write mock
MOCK_WRITE_SET_RETURN(10);  // Return 10 (success)

// Set up multiple sequential calls
MOCK_READ_SET_RETURN(0, "");           // First call: no data
MOCK_READ_SET_RETURN(12, "Router# : "); // Second call: prompt found
```

### Mock Verification

```c
// Check how many times functions were called
ASSERT_EQUAL(2, mock_serial_read.call_count);
ASSERT_EQUAL(1, mock_serial_write.call_count);

// Check what data was written
ASSERT_STRING_EQUAL("show version\n", mock_serial_write.last_data);
```

### Complete Mock Example

```c
TEST(send_command_success) {
    serial_conn_t conn;
    MOCK_INIT_ALL();

    // Set up mocks
    MOCK_WRITE_SET_RETURN(10);           // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found

    // Execute function under test
    int result = cisco_send_command(&conn, "show version", 5);

    // Verify results
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(1, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("show version\n", mock_serial_write.last_data);
    ASSERT_EQUAL(1, mock_serial_read.call_count);

    return 1;
}
```

## Assertions

The framework provides comprehensive assertion macros that automatically handle test failures:

### Basic Assertions

```c
ASSERT_TRUE(condition);        // Assert condition is true
ASSERT_FALSE(condition);       // Assert condition is false
ASSERT_EQUAL(expected, actual); // Assert two values are equal
ASSERT_STRING_EQUAL(expected, actual); // Assert two strings are equal
ASSERT_NOT_NULL(ptr);         // Assert pointer is not NULL
ASSERT_NULL(ptr);             // Assert pointer is NULL
```

### Assertion Examples

```c
// Integer comparisons
ASSERT_EQUAL(0, result);
ASSERT_EQUAL(42, calculate_answer());

// String comparisons
ASSERT_STRING_EQUAL("expected", actual_string);
ASSERT_STRING_EQUAL("Router# : ", prompt);

// Pointer checks
ASSERT_NOT_NULL(file_list);
ASSERT_NULL(error_ptr);

// Boolean conditions
ASSERT_TRUE(file_exists);
ASSERT_FALSE(connection_failed);
```

### Assertion Behavior

- All assertions automatically return `0` (test failure) if they fail
- Failed assertions print detailed error messages with file and line numbers
- Successful assertions continue execution normally

## Test Organization

### Test Fixtures

Use setup and teardown functions for common test preparation:

```c
// Define setup function
static void setup_serial_tests(void) {
    MOCK_INIT_ALL();
    // Other common setup
}

// Define teardown function
static void teardown_serial_tests(void) {
    // Cleanup resources
}

// Use in tests
TEST(my_test) {
    setup_serial_tests();

    // Test code here

    teardown_serial_tests();
    return 1;
}
```

### Test Suites

Organize related tests into suites:

```c
// Define a test suite
TEST_SUITE(serial_communication, setup_serial_tests, teardown_serial_tests);

// Run specific suite (future feature)
int run_test_suite(const char *suite_name);
```

### Test Grouping

Group related tests together in your test file:

```c
// ============================================================================
// cisco_send_command Tests
// ============================================================================

TEST(send_command_success) { /* ... */ }
TEST(send_command_failure) { /* ... */ }
TEST(send_command_timeout) { /* ... */ }

// ============================================================================
// cisco_wait_for_prompt Tests
// ============================================================================

TEST(wait_for_prompt_immediate) { /* ... */ }
TEST(wait_for_prompt_delayed) { /* ... */ }
```

## Build and Run

### Building Tests

```bash
# Build all tests
make all

# Build specific test
make test_cisco_commands
make test_file_tree
```

### Running Tests

```bash
# Run all tests
make test

# Run with verbose output
make test-verbose

# Run individual test executable
./test_cisco_commands
./test_file_tree
```

### Clean Up

```bash
# Remove test artifacts
make clean
```

### Available Make Targets

- `all` - Build all test executables
- `test` - Run all tests
- `test-verbose` - Run tests with verbose output
- `clean` - Remove test artifacts
- `help` - Show available targets

## Best Practices

### Test Naming

Use descriptive, specific test names:

```c
// Good
TEST(send_command_success_with_valid_input)
TEST(wait_for_prompt_timeout_after_5_seconds)
TEST(get_directory_listing_handles_empty_directory)

// Avoid
TEST(test1)
TEST(send_command)
TEST(timeout)
```

### Test Structure

Follow the Arrange-Act-Assert pattern:

```c
TEST(well_structured_test) {
    // Arrange - Set up test data and mocks
    serial_conn_t conn;
    MOCK_INIT_ALL();
    MOCK_READ_SET_RETURN(15, "Router# : ");

    // Act - Execute the function under test
    int result = cisco_send_command(&conn, "show version", 5);

    // Assert - Verify the results
    ASSERT_EQUAL(0, result);
    ASSERT_EQUAL(1, mock_serial_write.call_count);

    return 1;
}
```

### Mock Management

- Always initialize mocks at the start of each test
- Set up mock return values before calling functions under test
- Verify mock call counts and data in assertions
- Use descriptive mock setup comments

```c
TEST(proper_mock_usage) {
    // Initialize mocks
    MOCK_INIT_ALL();

    // Set up expected behavior
    MOCK_WRITE_SET_RETURN(10);           // serial_write succeeds
    MOCK_READ_SET_RETURN(15, "Router# : "); // prompt found immediately

    // Execute test
    int result = cisco_send_command(&conn, "test", 5);

    // Verify mock interactions
    ASSERT_EQUAL(1, mock_serial_write.call_count);
    ASSERT_STRING_EQUAL("test\n", mock_serial_write.last_data);
    ASSERT_EQUAL(1, mock_serial_read.call_count);

    return 1;
}
```

### Error Testing

Always test both success and failure cases:

```c
// Test success case
TEST(function_success) {
    MOCK_INIT_ALL();
    MOCK_WRITE_SET_RETURN(10);  // Success
    // ... test success path
    return 1;
}

// Test failure case
TEST(function_failure) {
    MOCK_INIT_ALL();
    MOCK_WRITE_SET_RETURN(-1);  // Failure
    // ... test failure path
    return 1;
}
```

### Resource Cleanup

Clean up allocated resources in tests:

```c
TEST(test_with_allocation) {
    file_entry_t *files = NULL;

    // Test code that allocates memory
    int result = cisco_get_directory_listing(&conn, "flash:/", &files, 30);

    // Clean up
    while (files) {
        file_entry_t *temp = files;
        files = files->next;
        free(temp);
    }

    return 1;
}
```

## Advanced Features

### Output Suppression

The framework automatically suppresses stdout during test execution to prevent printf output from functions under test from cluttering test results. This is handled transparently by the test runner.

### Test Statistics

The framework provides detailed statistics after test runs:

```
==================================================
TEST SUMMARY
==================================================
Total Tests: 42
Passed: 40
Failed: 2
Success Rate: 95.2%
==================================================
```

### Custom Mock Functions

For advanced testing scenarios, you can create custom mock functions:

```c
// Custom mock implementation
int custom_mock_function(serial_conn_t *conn, const char *data) {
    // Custom mock logic
    return mock_serial_write.return_values[mock_serial_write.call_count - 1];
}

// Override the real function
int serial_write(serial_conn_t *conn, const char *data) {
    return custom_mock_function(conn, data);
}
```

### Test Data Management

Use setup functions to manage test data:

```c
static char test_buffer[1024];
static serial_conn_t test_conn;

static void setup_test_data(void) {
    memset(test_buffer, 0, sizeof(test_buffer));
    memset(&test_conn, 0, sizeof(test_conn));
    MOCK_INIT_ALL();
}

TEST(test_with_data) {
    setup_test_data();

    // Use test_buffer and test_conn
    // ...

    return 1;
}
```

## Troubleshooting

### Common Issues

#### Test Not Running
- Ensure test is registered in `test_registry` array
- Check that test function returns 1 for success
- Verify test function follows correct signature

#### Mock Not Working
- Call `MOCK_INIT_ALL()` at start of test
- Set up mock return values before calling function under test
- Check that mock functions are properly overridden

#### Assertion Failures
- Use `ASSERT_EQUAL` for integer comparisons
- Use `ASSERT_STRING_EQUAL` for string comparisons
- Check that expected values match actual values

#### Build Errors
- Ensure all source files are compiled
- Check include paths in Makefile
- Verify all dependencies are available

### Debug Tips

1. **Add Debug Output**: Use `printf` statements (they'll be suppressed during test execution)
2. **Check Mock State**: Inspect `mock_serial_read.call_count` and `mock_serial_write.call_count`
3. **Verify Mock Data**: Check `mock_serial_write.last_data` for written content
4. **Test Isolation**: Ensure each test initializes its own mocks

### Getting Help

- Check existing test files for examples
- Review the framework source code in `test_framework.c`
- Look at the header file `test_framework.h` for available macros and functions

---

## Contributing

When adding new tests:

1. Follow the established naming conventions
2. Include both success and failure test cases
3. Use descriptive test names and comments
4. Clean up any allocated resources
5. Update the test registry arrays
6. Add test names to the parallel names array

## Framework Maintenance

The testing framework is designed to be:
- **Lightweight**: Minimal overhead and dependencies
- **Extensible**: Easy to add new assertion macros and mock functions
- **Maintainable**: Clean, well-documented code
- **Reliable**: Thoroughly tested itself

For questions or improvements to the framework itself, please refer to the project maintainers.
