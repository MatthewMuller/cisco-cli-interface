#include "test_framework.h"

// Global mock state
mock_serial_read_until_t mock_serial_read_until;

void mock_serial_read_until_init(void) {
    mock_serial_read_until.call_count = 0;
    mock_serial_read_until.return_index = 0;
    memset(mock_serial_read_until.return_values, 0, sizeof(mock_serial_read_until.return_values));
    memset(mock_serial_read_until.buffer_values, 0, sizeof(mock_serial_read_until.buffer_values));
}

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

// Test runner implementation
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
