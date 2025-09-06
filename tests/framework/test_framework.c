/**
 * @file test_framework_v2.c
 * @brief Implementation of the streamlined C test framework
 * 
 * This file contains the implementation of:
 * - Simple test registration and discovery
 * - Test runner with statistics
 * - Mock framework for serial communication testing
 * - Test fixtures for setup/teardown
 * 
 * @author Cisco CLI Interface Team
 * @version 2.0
 */

#include "test_framework.h"
#include "../include/cisco_cli.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

// ============================================================================
// Mock Framework Implementation
// ============================================================================

/** @brief Global mock state for serial functions */
mock_serial_t mock_serial_read;
mock_serial_t mock_serial_write;

/**
 * @brief Mock implementation of serial_read_until function
 */
int mock_serial_read_until_func(serial_conn_t *conn, char *buffer, int max_len, const char *delimiter) {
    (void)conn;
    (void)delimiter;
    
    mock_serial_read.call_count++;
    
    if (mock_serial_read.call_count <= mock_serial_read.return_index) {
        int index = mock_serial_read.call_count - 1;
        int return_value = mock_serial_read.return_values[index];
        
        if (return_value > 0 && buffer) {
            strncpy(buffer, mock_serial_read.buffer_values[index], max_len - 1);
            buffer[max_len - 1] = '\0';
        }
        
        return return_value;
    }
    
    return 0;
}

/**
 * @brief Mock implementation of serial_write function
 */
int mock_serial_write_func(serial_conn_t *conn, const char *data) {
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
    
    return 0;
}

// Override the real functions with our mocks
int serial_read_until(serial_conn_t *conn, char *buffer, int max_len, const char *delimiter) {
    return mock_serial_read_until_func(conn, buffer, max_len, delimiter);
}

int serial_write(serial_conn_t *conn, const char *data) {
    return mock_serial_write_func(conn, data);
}

// ============================================================================
// Test Fixtures
// ============================================================================

/**
 * @brief Common test setup for serial tests
 */
TEST_SETUP(serial) {
    MOCK_INIT_ALL();
}

/**
 * @brief Common test teardown for serial tests
 */
TEST_TEARDOWN(serial) {
    // Clean up any resources if needed
}

// ============================================================================
// Test Registration and Discovery
// ============================================================================

// Global test statistics
static struct {
    int total_tests;
    int passed_tests;
    int failed_tests;
    int current_suite_passed;
    int current_suite_total;
} test_stats = {0};

// Test registry - this will be populated by the test files
extern test_func_t test_registry[];
extern int test_registry_count;

// Test names registry - parallel array to test_registry
extern const char* test_names[];
extern int test_names_count;

/**
 * @brief Reset test statistics
 */
static void reset_test_stats(void) {
    test_stats.total_tests = 0;
    test_stats.passed_tests = 0;
    test_stats.failed_tests = 0;
    test_stats.current_suite_passed = 0;
    test_stats.current_suite_total = 0;
}

/**
 * @brief Print test statistics
 */
void print_test_stats(void) {
    printf("\n==================================================\n");
    printf("TEST SUMMARY\n");
    printf("==================================================\n");
    printf("Total Tests: %d\n", test_stats.total_tests);
    printf("Passed: %d\n", test_stats.passed_tests);
    printf("Failed: %d\n", test_stats.failed_tests);
    printf("Success Rate: %.1f%%\n", 
           test_stats.total_tests > 0 ? 
           (float)test_stats.passed_tests / test_stats.total_tests * 100.0f : 0.0f);
    printf("==================================================\n");
}

/**
 * @brief Run a single test function
 */
static int run_single_test(const char *test_name, test_func_t test_func) {
    printf("  Running: %s", test_name);
    fflush(stdout);
    
    // Suppress stdout during test execution to avoid printf output from functions under test
    suppress_stdout();
    
    int result = test_func();
    
    // Restore stdout after test execution
    restore_stdout();
    
    if (result) {
        printf(" ✓ PASS\n");
        test_stats.passed_tests++;
        test_stats.current_suite_passed++;
    } else {
        printf(" ✗ FAIL\n");
        test_stats.failed_tests++;
    }
    
    test_stats.total_tests++;
    test_stats.current_suite_total++;
    
    return result;
}

/**
 * @brief Run all registered tests
 */
int run_all_tests(void) {
    printf("Running all tests...\n");
    printf("==================================================\n");
    
    reset_test_stats();
    
    // Check if test registry is available
    if (test_registry_count <= 0) {
        printf("No tests found in registry.\n");
        return 1;
    }
    
    int all_passed = 1;
    
    for (int i = 0; i < test_registry_count; i++) {
        const char* test_name;
        
        // Use test names if available, otherwise fall back to generic names
        if (i < test_names_count && test_names[i]) {
            test_name = test_names[i];
        } else {
            static char generic_name[64];
            snprintf(generic_name, sizeof(generic_name), "test_%d", i);
            test_name = generic_name;
        }
        
        if (!run_single_test(test_name, test_registry[i])) {
            all_passed = 0;
        }
    }
    
    print_test_stats();
    
    return all_passed;
}

/**
 * @brief Run tests in a specific suite
 */
int run_test_suite(const char *suite_name) {
    printf("Running test suite: %s\n", suite_name);
    printf("------------------------------\n");
    
    reset_test_stats();
    
    // For now, run all tests since we don't have suite organization yet
    // In a full implementation, you'd filter by suite
    return run_all_tests();
}

// ============================================================================
// Output Suppression Functions
// ============================================================================

// Global variables to store original stdout
static int original_stdout_fd = -1;
static int stdout_suppressed = 0;

/**
 * @brief Suppress stdout output during test execution
 */
void suppress_stdout(void) {
    if (!stdout_suppressed) {
        // Store the original stdout file descriptor
        original_stdout_fd = dup(STDOUT_FILENO);
        
        // Redirect stdout to /dev/null to suppress output
        freopen("/dev/null", "w", stdout);
        stdout_suppressed = 1;
    }
}

/**
 * @brief Restore stdout output after test execution
 */
void restore_stdout(void) {
    if (stdout_suppressed && original_stdout_fd != -1) {
        // Close the current stdout (which points to /dev/null)
        fclose(stdout);
        
        // Restore the original stdout file descriptor
        dup2(original_stdout_fd, STDOUT_FILENO);
        close(original_stdout_fd);
        original_stdout_fd = -1;
        
        // Reopen stdout as a FILE* for the restored file descriptor
        stdout = fopen("/dev/stdout", "w");
        stdout_suppressed = 0;
    }
}

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * @brief Trim whitespace from string (mock implementation)
 */
void trim_whitespace(char *str) {
    (void)str;
    // Simple implementation for testing
}

/**
 * @brief Check if file is binary (mock implementation)
 */
int is_binary_file(const char *filename) {
    if (!filename) return 0;
    
    // Check for common binary file extensions
    const char *ext = strrchr(filename, '.');
    if (ext) {
        if (strcmp(ext, ".bin") == 0 || 
            strcmp(ext, ".exe") == 0 || 
            strcmp(ext, ".dll") == 0 || 
            strcmp(ext, ".so") == 0 || 
            strcmp(ext, ".dylib") == 0 ||
            strcmp(ext, ".o") == 0 ||
            strcmp(ext, ".a") == 0) {
            return 1; // Binary file
        }
    }
    
    return 0; // Not binary
}
