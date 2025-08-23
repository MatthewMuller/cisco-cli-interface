# Testing Framework Documentation

This document describes the simple unit testing and mocking framework for the Cisco CLI Interface project.

## Overview

The testing framework provides:
- Simple test macros for assertions
- Mock serial communication functions
- Test runner with colored output
- Integration with Docker development environment

## Quick Start

### Running Tests

From the project root directory:

```bash
# Run tests directly
./scripts/run_tests.sh

# Or use make
make test
./build/test-runner

# Or use Docker
./scripts/dev-setup.sh test
```

### Test Output

The test runner provides colored output:
- 🔵 Blue: Test information
- 🟢 Green: Passed tests
- 🔴 Red: Failed tests
- 🟡 Yellow: Test suite headers

## Writing Tests

### Test Structure

Each test function should follow this pattern:

```c
void test_function_name_scenario(void) {
    TEST_START("function_name_scenario");
    
    // Setup
    serial_conn_t conn = {0};
    mock_serial_reset();
    
    // Configure mock behavior
    mock_serial_set_read_data("expected response\nRouter# : ");
    mock_serial_set_expected_write("expected command\n");
    
    // Execute function under test
    int result = cisco_send_command(&conn, "test_command", 30);
    
    // Assertions
    TEST_ASSERT_EQ(0, result);
    TEST_ASSERT_EQ(1, mock_serial.write_called);
    
    TEST_PASS();
}
```

### Available Test Macros

#### Basic Assertions
- `TEST_ASSERT(condition)` - Assert that condition is true
- `TEST_ASSERT_EQ(expected, actual)` - Assert equality
- `TEST_ASSERT_STR_EQ(expected, actual)` - Assert string equality
- `TEST_ASSERT_NULL(ptr)` - Assert pointer is NULL
- `TEST_ASSERT_NOT_NULL(ptr)` - Assert pointer is not NULL

#### Test Flow
- `TEST_START(name)` - Start a test with the given name
- `TEST_PASS()` - Mark test as passed
- `TEST_SUITE_START(name)` - Start a test suite
- `TEST_SUITE_END()` - End test suite and show results

### Mock Functions

The framework provides mock implementations of serial communication functions:

#### Mock Setup Functions
```c
void mock_serial_init(void);                    // Initialize mock
void mock_serial_reset(void);                   // Reset mock state
void mock_serial_set_read_data(const char *data); // Set data to return on read
void mock_serial_set_expected_write(const char *data); // Set expected write data
void mock_serial_set_return_value(int value);   // Set return value for operations
void mock_serial_set_timeout(int timeout);      // Simulate timeout
```

#### Mock Serial Functions
- `mock_serial_write()` - Mock serial write operation
- `mock_serial_read()` - Mock serial read operation  
- `mock_serial_read_until()` - Mock serial read until delimiter

### Mock State Inspection

After running tests, you can inspect the mock state:

```c
// Check how many times write was called
TEST_ASSERT_EQ(1, mock_serial.write_called);

// Check what was last written
TEST_ASSERT_STR_EQ("expected command\n", mock_serial.last_written);

// Check if expected write matched
if (mock_serial.expected_write[0] != '\0') {
    // Verify expected write was called
}
```

## Example Tests

### Testing Success Scenarios

```c
void test_cisco_send_command_success(void) {
    TEST_START("cisco_send_command_success");
    
    serial_conn_t conn = {0};
    mock_serial_reset();
    mock_serial_set_read_data("Command output\nRouter# : ");
    mock_serial_set_expected_write("test_command\n");
    
    int result = cisco_send_command(&conn, "test_command", 30);
    
    TEST_ASSERT_EQ(0, result);
    TEST_ASSERT_EQ(1, mock_serial.write_called);
    TEST_PASS();
}
```

### Testing Failure Scenarios

```c
void test_cisco_send_command_failure(void) {
    TEST_START("cisco_send_command_failure");
    
    serial_conn_t conn = {0};
    mock_serial_reset();
    mock_serial_set_return_value(-1); // Simulate write failure
    
    int result = cisco_send_command(&conn, "test_command", 30);
    
    TEST_ASSERT_EQ(-1, result);
    TEST_PASS();
}
```

### Testing Timeout Scenarios

```c
void test_cisco_wait_for_prompt_timeout(void) {
    TEST_START("cisco_wait_for_prompt_timeout");
    
    serial_conn_t conn = {0};
    mock_serial_reset();
    mock_serial_set_timeout(1);
    mock_serial_set_read_data("Some output without prompt\n");
    
    int result = cisco_wait_for_prompt(&conn, 30);
    
    TEST_ASSERT_EQ(-1, result);
    TEST_PASS();
}
```

### Testing Complex Data Parsing

```c
void test_cisco_get_directory_listing_success(void) {
    TEST_START("cisco_get_directory_listing_success");
    
    serial_conn_t conn = {0};
    file_entry_t *files = NULL;
    mock_serial_reset();
    
    const char *dir_output = 
        "Directory of flash:/\n"
        "2  -rwx  1429      Jan 01 2020 00:00:00  test.txt\n"
        "3  drwx  0         Jan 01 2020 00:00:00  config\n"
        "Router# : ";
    
    mock_serial_set_read_data(dir_output);
    mock_serial_set_expected_write("dir flash:/\n");
    
    int result = cisco_get_directory_listing(&conn, "flash:/", &files, 30);
    
    TEST_ASSERT_EQ(2, result);
    TEST_ASSERT_NOT_NULL(files);
    TEST_ASSERT_STR_EQ("test.txt", files->name);
    TEST_ASSERT_EQ(FILE_TYPE_REGULAR, files->type);
    
    // Clean up allocated memory
    while (files) {
        file_entry_t *next = files->next;
        free(files);
        files = next;
    }
    
    TEST_PASS();
}
```

## Adding New Tests

1. **Add test function declaration** to `include/test_framework.h`:
   ```c
   void test_new_function_scenario(void);
   ```

2. **Implement the test** in `src/test_framework.c`:
   ```c
   void test_new_function_scenario(void) {
       TEST_START("new_function_scenario");
       // Test implementation
       TEST_PASS();
   }
   ```

3. **Add to test runner** in `src/test_framework.c`:
   ```c
   void run_all_tests(void) {
       // ... existing tests ...
       test_new_function_scenario();
       test_ctx.total_tests++;
   }
   ```

## Integration with Docker

The testing framework is integrated with the Docker development environment:

```bash
# Build and run tests in Docker
./scripts/dev-setup.sh test

# Or start a development shell and run tests manually
./scripts/dev-setup.sh shell
# Inside container:
make test
./build/test-runner
```

## Best Practices

1. **Reset mocks** at the start of each test
2. **Set up expected behavior** before calling functions under test
3. **Clean up allocated memory** in tests that create data structures
4. **Use descriptive test names** that indicate the scenario being tested
5. **Test both success and failure cases**
6. **Verify mock interactions** to ensure functions behave as expected

## Troubleshooting

### Common Issues

1. **Tests failing with timeout**: Check that mock data includes expected prompts
2. **Memory leaks**: Ensure all allocated memory is freed in tests
3. **Mock not working**: Verify that mock functions are properly defined and linked
4. **Build errors**: Check that all test files are included in the Makefile

### Debug Output

The framework provides debug output for mock mismatches:
```
Mock serial write mismatch: expected 'expected_command\n', got 'actual_command\n'
```

## Extending the Framework

### Adding New Mock Functions

1. Add function declaration to `include/test_framework.h`
2. Implement in `src/test_framework.c`
3. Add state variables to `mock_serial_t` if needed

### Adding New Assertion Macros

1. Define macro in `include/test_framework.h`
2. Use `test_ctx.failed_tests++` for failures
3. Use `test_ctx.current_test_name` for error messages

### Adding New Test Types

1. Create new test suite functions
2. Add to `run_all_tests()` function
3. Update test count in `test_ctx.total_tests`
