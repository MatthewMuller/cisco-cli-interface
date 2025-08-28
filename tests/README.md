# Unit Tests for Cisco CLI Interface

This directory contains unit tests for the Cisco CLI interface functions.

## Structure

- `test_framework.h` - Simple test framework with assertion macros and mock support
- `test_framework.c` - Implementation of the test framework
- `test_cisco_commands.c` - Unit tests for the `cisco_wait_for_prompt` function
- `Makefile` - Build configuration for the tests

## How to Run Tests

1. Build the tests:
   ```bash
   cd tests
   make
   ```

2. Run the tests:
   ```bash
   make test
   ```

3. Clean up:
   ```bash
   make clean
   ```

## Test Framework

The test framework provides:

- **Simple assertions**: `TEST_ASSERT`, `TEST_ASSERT_EQUAL`, `TEST_ASSERT_STRING_EQUAL`
- **Mock support**: Easy mocking of functions like `serial_read_until`
- **Test runner**: Automatic test execution and reporting

## Mocking

The framework includes a simple mock system for `serial_read_until`:

```c
// Initialize mock
mock_serial_read_until_init();

// Set up expected return values
mock_serial_read_until_set_return(15, "Router# : ");

// Call the function under test
int result = cisco_wait_for_prompt(&conn, 10);

// Assert results
TEST_ASSERT_EQUAL(0, result);
TEST_ASSERT_EQUAL(1, mock_serial_read_until.call_count);
```

## Adding New Tests

To add tests for other functions:

1. Create a new test file (e.g., `test_other_function.c`)
2. Include the test framework header
3. Mock any external dependencies
4. Write test functions that return 1 for pass, 0 for fail
5. Add the test to the main function
6. Update the Makefile to include the new test file

## Example Test

```c
int test_my_function_success(void) {
    // Setup
    mock_serial_read_until_init();
    mock_serial_read_until_set_return(10, "test data");
    
    // Execute
    int result = my_function(&conn);
    
    // Assert
    TEST_ASSERT_EQUAL(0, result);
    
    return 1; // Pass
}
```
