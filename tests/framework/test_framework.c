#include "test_framework.h"
#include <unistd.h>
#include <sys/time.h>

// Utility functions needed for testing
void trim_whitespace(char *str) {
    char *end;
    
    // Trim leading space
    while (isspace((unsigned char)*str)) str++;
    
    if (*str == 0) return; // All spaces
    
    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    
    // Write new null terminator
    end[1] = '\0';
}

int is_binary_file(const char *filename) {
    const char *ext = strrchr(filename, '.');
    if (!ext) return 0;
    
    // Common binary file extensions
    const char *binary_extensions[] = {
        ".bin", ".exe", ".com", ".dll", ".so", ".dylib", ".o", ".a", ".lib",
        ".img", ".iso", ".tar", ".gz", ".zip", ".rar", ".7z", ".bz2",
        ".pcap", ".pcapng", ".cap", ".cfg", ".conf", ".config"
    };
    
    int num_extensions = sizeof(binary_extensions) / sizeof(binary_extensions[0]);
    for (int i = 0; i < num_extensions; i++) {
        if (strcasecmp(ext, binary_extensions[i]) == 0) {
            return 1;
        }
    }
    
    return 0;
}

// Global test context
test_context_t test_ctx = {0};

// Global mock serial state
mock_serial_t mock_serial = {0};

// Mock serial implementation
void mock_serial_init(void) {
    mock_serial_reset();
}

void mock_serial_reset(void) {
    if (mock_serial.read_data) {
        free(mock_serial.read_data);
    }
    memset(&mock_serial, 0, sizeof(mock_serial));
    mock_serial.return_value = 0;
}

void mock_serial_set_read_data(const char *data) {
    if (mock_serial.read_data) {
        free(mock_serial.read_data);
    }
    if (data) {
        mock_serial.read_data_len = strlen(data);
        mock_serial.read_data = malloc(mock_serial.read_data_len + 1);
        strcpy(mock_serial.read_data, data);
    } else {
        mock_serial.read_data = NULL;
        mock_serial.read_data_len = 0;
    }
    mock_serial.read_data_pos = 0;
}

void mock_serial_set_expected_write(const char *data) {
    if (data) {
        strncpy(mock_serial.expected_write, data, sizeof(mock_serial.expected_write) - 1);
        mock_serial.expected_write[sizeof(mock_serial.expected_write) - 1] = '\0';
    } else {
        mock_serial.expected_write[0] = '\0';
    }
}

void mock_serial_set_return_value(int value) {
    mock_serial.return_value = value;
}

void mock_serial_set_timeout(int timeout) {
    mock_serial.timeout_occurred = timeout;
}

// Mock implementations of serial functions
int mock_serial_write(serial_conn_t *conn, const char *data) {
    (void)conn; // Unused parameter
    
    mock_serial.write_called++;
    strncpy(mock_serial.last_written, data, sizeof(mock_serial.last_written) - 1);
    mock_serial.last_written[sizeof(mock_serial.last_written) - 1] = '\0';
    
    // Check if expected write matches
    if (mock_serial.expected_write[0] != '\0' && 
        strcmp(mock_serial.expected_write, data) != 0) {
        printf("Mock serial write mismatch: expected '%s', got '%s'\n", 
               mock_serial.expected_write, data);
    }
    
    return mock_serial.return_value;
}

int mock_serial_read(serial_conn_t *conn, char *buffer, int max_len) {
    (void)conn; // Unused parameter
    
    if (!mock_serial.read_data || mock_serial.read_data_pos >= mock_serial.read_data_len) {
        return -1; // No more data
    }
    
    int remaining = mock_serial.read_data_len - mock_serial.read_data_pos;
    int to_copy = (remaining < max_len) ? remaining : max_len;
    
    memcpy(buffer, mock_serial.read_data + mock_serial.read_data_pos, to_copy);
    mock_serial.read_data_pos += to_copy;
    
    return to_copy;
}

int mock_serial_read_until(serial_conn_t *conn, char *buffer, int max_len, const char *delimiter) {
    (void)conn; // Unused parameter
    
    if (mock_serial.timeout_occurred) {
        usleep(100000); // Simulate delay
        return -1; // Timeout
    }
    
    if (!mock_serial.read_data || mock_serial.read_data_pos >= mock_serial.read_data_len) {
        return -1; // No more data
    }
    
    // Find delimiter in remaining data
    char *delim_pos = strstr(mock_serial.read_data + mock_serial.read_data_pos, delimiter);
    if (!delim_pos) {
        // No delimiter found, return all remaining data
        int remaining = mock_serial.read_data_len - mock_serial.read_data_pos;
        int to_copy = (remaining < max_len - 1) ? remaining : max_len - 1;
        
        memcpy(buffer, mock_serial.read_data + mock_serial.read_data_pos, to_copy);
        buffer[to_copy] = '\0';
        mock_serial.read_data_pos += to_copy;
        
        return to_copy;
    }
    
    // Copy data up to delimiter
    int data_len = delim_pos - (mock_serial.read_data + mock_serial.read_data_pos);
    int to_copy = (data_len < max_len - 1) ? data_len : max_len - 1;
    
    memcpy(buffer, mock_serial.read_data + mock_serial.read_data_pos, to_copy);
    buffer[to_copy] = '\0';
    mock_serial.read_data_pos += data_len + strlen(delimiter);
    
    return to_copy;
}

// Test implementations
void test_cisco_wait_for_prompt_success(void) {
    TEST_START("cisco_wait_for_prompt_success");
    
    serial_conn_t conn = {0};
    mock_serial_reset();
    mock_serial_set_read_data("Some output\nRouter# : ");
    
    int result = cisco_wait_for_prompt(&conn);
    
    TEST_ASSERT_EQ(0, result);
    TEST_PASS();
}

void test_cisco_wait_for_prompt_timeout(void) {
    TEST_START("cisco_wait_for_prompt_timeout");
    
    serial_conn_t conn = {0};
    mock_serial_reset();
    mock_serial_set_timeout(1);
    mock_serial_set_read_data("Some output without prompt\n");
    
    int result = cisco_wait_for_prompt(&conn);
    
    TEST_ASSERT_EQ(-1, result);
    TEST_PASS();
}

void test_cisco_send_command_success(void) {
    TEST_START("cisco_send_command_success");
    
    serial_conn_t conn = {0};
    mock_serial_reset();
    mock_serial_set_read_data("Command output\nRouter# : ");
    mock_serial_set_expected_write("test_command\n");
    
    int result = cisco_send_command(&conn, "test_command");
    
    TEST_ASSERT_EQ(0, result);
    TEST_ASSERT_EQ(1, mock_serial.write_called);
    TEST_PASS();
}

void test_cisco_send_command_failure(void) {
    TEST_START("cisco_send_command_failure");
    
    serial_conn_t conn = {0};
    mock_serial_reset();
    mock_serial_set_return_value(-1); // Simulate write failure
    
    int result = cisco_send_command(&conn, "test_command");
    
    TEST_ASSERT_EQ(-1, result);
    TEST_PASS();
}

void test_cisco_init_flash_success(void) {
    TEST_START("cisco_init_flash_success");
    
    serial_conn_t conn = {0};
    mock_serial_reset();
    mock_serial_set_read_data("flash_init\nRouter# : ");
    mock_serial_set_expected_write("flash_init\n");
    
    int result = cisco_init_flash(&conn);
    
    TEST_ASSERT_EQ(0, result);
    TEST_PASS();
}

void test_cisco_init_flash_timeout(void) {
    TEST_START("cisco_init_flash_timeout");
    
    serial_conn_t conn = {0};
    mock_serial_reset();
    mock_serial_set_timeout(1);
    mock_serial_set_read_data("Some other output\n");
    
    int result = cisco_init_flash(&conn);
    
    TEST_ASSERT_EQ(-1, result);
    TEST_PASS();
}

void test_cisco_get_directory_listing_success(void) {
    TEST_START("cisco_get_directory_listing_success");
    
    serial_conn_t conn = {0};
    file_entry_t *files = NULL;
    mock_serial_reset();
    
    // First read: command response with directory listing
    const char *dir_output = 
        "Directory of flash:/\n"
        "2  -rwx  1429      Jan 01 2020 00:00:00  test.txt\n"
        "3  drwx  0         Jan 01 2020 00:00:00  config\n";
    
    // Second read: prompt after directory listing
    const char *prompt_output = "Router# : ";
    
    // Set up mock to return directory listing first, then prompt
    char combined_output[2048];
    snprintf(combined_output, sizeof(combined_output), "%s%s", dir_output, prompt_output);
    mock_serial_set_read_data(combined_output);
    mock_serial_set_expected_write("dir flash:/\n");
    
    int result = cisco_get_directory_listing(&conn, "flash:/", &files);
    
    TEST_ASSERT_EQ(2, result);
    TEST_ASSERT_NOT_NULL(files);
    TEST_ASSERT_STR_EQ("test.txt", files->name);
    TEST_ASSERT_EQ(FILE_TYPE_REGULAR, files->type);
    TEST_ASSERT_NOT_NULL(files->next);
    TEST_ASSERT_STR_EQ("config", files->next->name);
    TEST_ASSERT_EQ(FILE_TYPE_DIRECTORY, files->next->type);
    
    // Clean up
    while (files) {
        file_entry_t *next = files->next;
        free(files);
        files = next;
    }
    
    TEST_PASS();
}

void test_cisco_get_directory_listing_empty(void) {
    TEST_START("cisco_get_directory_listing_empty");
    
    serial_conn_t conn = {0};
    file_entry_t *files = NULL;
    mock_serial_reset();
    
    const char *dir_output = 
        "Directory of flash:/\n"
        "No files found\n"
        "Router# : ";
    
    mock_serial_set_read_data(dir_output);
    mock_serial_set_expected_write("dir flash:/\n");
    
    int result = cisco_get_directory_listing(&conn, "flash:/", &files);
    
    TEST_ASSERT_EQ(0, result);
    TEST_ASSERT_NULL(files);
    TEST_PASS();
}

void test_cisco_delete_file_success(void) {
    TEST_START("cisco_delete_file_success");
    
    serial_conn_t conn = {0};
    mock_serial_reset();
    
    // First read: confirmation prompt
    const char *confirm_output = "Are you sure you want to delete 'test.txt'? [confirm] ";
    
    // Second read: success message
    const char *success_output = "Deleted file 'test.txt'\nRouter# : ";
    
    // Combine the outputs
    char combined_output[2048];
    snprintf(combined_output, sizeof(combined_output), "%s%s", confirm_output, success_output);
    mock_serial_set_read_data(combined_output);
    mock_serial_set_expected_write("delete test.txt\n");
    
    int result = cisco_delete_file(&conn, "test.txt");
    
    TEST_ASSERT_EQ(0, result);
    TEST_PASS();
}

void test_cisco_delete_file_failure(void) {
    TEST_START("cisco_delete_file_failure");
    
    serial_conn_t conn = {0};
    mock_serial_reset();
    
    const char *delete_output = 
        "Are you sure you want to delete 'nonexistent.txt'? [confirm] y\n"
        "File 'nonexistent.txt' not deleted\n"
        "Router# : ";
    
    mock_serial_set_read_data(delete_output);
    
    int result = cisco_delete_file(&conn, "nonexistent.txt");
    
    TEST_ASSERT_EQ(-1, result);
    TEST_PASS();
}

void test_cisco_delete_directory_success(void) {
    TEST_START("cisco_delete_directory_success");
    
    serial_conn_t conn = {0};
    mock_serial_reset();
    
    // First read: confirmation prompt
    const char *confirm_output = "Are you sure you want to delete 'test_dir'? [confirm] ";
    
    // Second read: success message
    const char *success_output = "Directory 'test_dir' removed\nRouter# : ";
    
    // Combine the outputs
    char combined_output[2048];
    snprintf(combined_output, sizeof(combined_output), "%s%s", confirm_output, success_output);
    mock_serial_set_read_data(combined_output);
    mock_serial_set_expected_write("rmdir test_dir\n");
    
    int result = cisco_delete_directory(&conn, "test_dir");
    
    TEST_ASSERT_EQ(0, result);
    TEST_PASS();
}

void test_cisco_delete_directory_failure(void) {
    TEST_START("cisco_delete_directory_failure");
    
    serial_conn_t conn = {0};
    mock_serial_reset();
    
    const char *delete_output = 
        "Are you sure you want to delete 'nonexistent_dir'? [confirm] y\n"
        "Directory 'nonexistent_dir' not removed\n"
        "Router# : ";
    
    mock_serial_set_read_data(delete_output);
    
    int result = cisco_delete_directory(&conn, "nonexistent_dir");
    
    TEST_ASSERT_EQ(-1, result);
    TEST_PASS();
}

// Test runner
void run_all_tests(void) {
    mock_serial_init();
    
    TEST_SUITE_START("Cisco Commands");
    
    // Run all tests
    test_cisco_wait_for_prompt_success();
    test_cisco_wait_for_prompt_timeout();
    test_cisco_send_command_success();
    test_cisco_send_command_failure();
    test_cisco_init_flash_success();
    test_cisco_init_flash_timeout();
    test_cisco_get_directory_listing_success();
    test_cisco_get_directory_listing_empty();
    test_cisco_delete_file_success();
    test_cisco_delete_file_failure();
    test_cisco_delete_directory_success();
    test_cisco_delete_directory_failure();
    
    test_ctx.total_tests = 12;
    
    TEST_SUITE_END();
    
    mock_serial_reset();
}
