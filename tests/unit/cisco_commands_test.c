#include "../../include/cisco_cli.h"
#include "../framework/test_framework.h"

// Define mock functions to override real serial functions
#define serial_write mock_serial_write
#define serial_read mock_serial_read
#define serial_read_until mock_serial_read_until

// Include the original cisco_commands.c content with mock overrides
int cisco_wait_for_prompt(serial_conn_t *conn) {
    char buffer[MAX_LINE_LEN];
    int timeout = 30; // 30 seconds timeout
    
    while (timeout > 0) {
        int bytes_read = serial_read_until(conn, buffer, sizeof(buffer), "\n");
        if (bytes_read > 0) {
            // Look for prompt pattern (ends with ": ")
            if (strstr(buffer, ": ") != NULL) {
                return 0;
            }
        }
        sleep(1); // 1 second delay (simplified for testing)
        timeout--;
    }
    
    return -1; // Timeout
}

int cisco_send_command(serial_conn_t *conn, const char *command) {
    char cmd_buffer[MAX_LINE_LEN];
    
    // Format command with newline
    snprintf(cmd_buffer, sizeof(cmd_buffer), "%s\n", command);
    
    // Send command
    if (serial_write(conn, cmd_buffer) < 0) {
        return -1;
    }
    
    // Wait for response and prompt
    return cisco_wait_for_prompt(conn);
}

int cisco_init_flash(serial_conn_t *conn) {
    char buffer[MAX_LINE_LEN];
    int timeout = 60; // 60 seconds timeout for flash init
    
    // Wait for the flash_init prompt
    while (timeout > 0) {
        int bytes_read = serial_read_until(conn, buffer, sizeof(buffer), "\n");
        if (bytes_read > 0) {
            if (strstr(buffer, "flash_init") != NULL) {
                break;
            }
        }
        sleep(1); // 1 second delay (simplified for testing)
        timeout--;
    }
    
    if (timeout <= 0) {
        return -1; // Timeout waiting for flash_init prompt
    }
    
    // Send flash_init command
    if (cisco_send_command(conn, "flash_init") < 0) {
        return -1;
    }
    
    return 0;
}

int cisco_get_directory_listing(serial_conn_t *conn, const char *path, file_entry_t **files) {
    char command[MAX_LINE_LEN];
    char buffer[MAX_LINE_LEN * 10]; // Large buffer for directory listing
    char *line, *saveptr;
    int file_count = 0;
    
    // Build command
    if (strcmp(path, "flash:/") == 0) {
        strcpy(command, "dir flash:/");
    } else {
        snprintf(command, sizeof(command), "dir %s", path);
    }
    
    // Send command
    if (cisco_send_command(conn, command) < 0) {
        return -1;
    }
    
    // Read response
    int bytes_read = serial_read_until(conn, buffer, sizeof(buffer), "\n");
    if (bytes_read <= 0) {
        return -1;
    }
    
    // Parse directory listing
    *files = NULL;
    file_entry_t *last_file = NULL;
    
    line = strtok_r(buffer, "\n", &saveptr);
    while (line != NULL) {
        trim_whitespace(line);
        
        // Skip header lines and empty lines
        if (strlen(line) == 0 || 
            strstr(line, "Directory of") != NULL ||
            strstr(line, "bytes available") != NULL ||
            strstr(line, "List of filesystems") != NULL) {
            line = strtok_r(NULL, "\n", &saveptr);
            continue;
        }
        
        // Parse file entry (format: "2  -rwx  1429      <date>               filename")
        int file_num, size;
        char permissions[16], filename[MAX_PATH_LEN];
        
        if (sscanf(line, "%d %s %d %*s %*s %*s %*s %*s %[^\n]", 
                   &file_num, permissions, &size, filename) >= 3) {
            
            // Create file entry
            file_entry_t *file = malloc(sizeof(file_entry_t));
            if (!file) continue;
            
            strncpy(file->name, filename, MAX_PATH_LEN - 1);
            file->name[MAX_PATH_LEN - 1] = '\0';
            
            // Build full path
            if (strcmp(path, "flash:/") == 0) {
                if (snprintf(file->full_path, MAX_PATH_LEN, "flash:/%s", filename) >= MAX_PATH_LEN) {
                    // Truncation occurred, ensure null termination
                    file->full_path[MAX_PATH_LEN - 1] = '\0';
                }
            } else {
                if (snprintf(file->full_path, MAX_PATH_LEN, "%s/%s", path, filename) >= MAX_PATH_LEN) {
                    // Truncation occurred, ensure null termination
                    file->full_path[MAX_PATH_LEN - 1] = '\0';
                }
            }
            
            // Determine file type based on permissions and name
            if (strstr(permissions, "d") != NULL) {
                file->type = FILE_TYPE_DIRECTORY;
            } else if (is_binary_file(filename)) {
                file->type = FILE_TYPE_BINARY;
            } else {
                file->type = FILE_TYPE_REGULAR;
            }
            
            file->size = size;
            file->selected = 0;
            file->next = NULL;
            
            // Add to list
            if (*files == NULL) {
                *files = file;
            } else {
                last_file->next = file;
            }
            last_file = file;
            file_count++;
        }
        
        line = strtok_r(NULL, "\n", &saveptr);
    }
    
    return file_count;
}

int cisco_delete_file(serial_conn_t *conn, const char *file_path) {
    char command[MAX_LINE_LEN];
    char buffer[MAX_LINE_LEN];
    
    // Build delete command
    snprintf(command, sizeof(command), "delete %s", file_path);
    
    // Send delete command
    if (cisco_send_command(conn, command) < 0) {
        return -1;
    }
    
    // Wait for confirmation prompt
    int bytes_read = serial_read_until(conn, buffer, sizeof(buffer), "\n");
    if (bytes_read > 0 && strstr(buffer, "Are you sure") != NULL) {
        // Send confirmation
        if (serial_write(conn, "y\n") < 0) {
            return -1;
        }
        
        // Wait for result
        bytes_read = serial_read_until(conn, buffer, sizeof(buffer), "\n");
        if (bytes_read > 0) {
            if (strstr(buffer, "deleted") != NULL) {
                return 0; // Success
            } else if (strstr(buffer, "not deleted") != NULL) {
                return -1; // Failed
            }
        }
    }
    
    return -1;
}

int cisco_delete_directory(serial_conn_t *conn, const char *dir_path) {
    char command[MAX_LINE_LEN];
    char buffer[MAX_LINE_LEN];
    
    // Build rmdir command
    snprintf(command, sizeof(command), "rmdir %s", dir_path);
    
    // Send rmdir command
    if (cisco_send_command(conn, command) < 0) {
        return -1;
    }
    
    // Wait for confirmation prompt
    int bytes_read = serial_read_until(conn, buffer, sizeof(buffer), "\n");
    if (bytes_read > 0 && strstr(buffer, "Are you sure") != NULL) {
        // Send confirmation
        if (serial_write(conn, "y\n") < 0) {
            return -1;
        }
        
        // Wait for result
        bytes_read = serial_read_until(conn, buffer, sizeof(buffer), "\n");
        if (bytes_read > 0) {
            if (strstr(buffer, "removed") != NULL) {
                return 0; // Success
            } else if (strstr(buffer, "not removed") != NULL) {
                return -1; // Failed
            }
        }
    }
    
    return -1;
}
