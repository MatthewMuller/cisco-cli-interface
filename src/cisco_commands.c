#include "cisco_cli.h"

/**
 * @brief Waits for a Cisco device prompt to appear on the serial connection
 * 
 * This function continuously reads from the serial connection until it detects
 * a Cisco device prompt (typically ending with ": "). It's used to synchronize
 * communication with the Cisco device and ensure the device is ready to receive
 * the next command.
 * 
 * @param conn Pointer to the serial connection structure
 * @param timeout Maximum time to wait in seconds before giving up
 * 
 * @return 0 on success (prompt found), -1 on timeout or error
 * 
 * @note The function sleeps for 1 second between read attempts to avoid
 *       excessive CPU usage while waiting for the prompt.
 */
int cisco_wait_for_prompt(serial_conn_t *conn, int timeout) {
    char buffer[MAX_LINE_LEN];
    
    // timeout parameter is in seconds
    while (timeout > 0) {
        int bytes_read = serial_read_until(conn, buffer, sizeof(buffer), "\n");
        if (bytes_read > 0) {
            // Look for prompt pattern (ends with ": ")
            if (strstr(buffer, ": ") != NULL) {
                return 0;
            }
        }
        usleep(1000000); // 1 second delay
        timeout--;
    }
    
    return -1; // Timeout
}

/**
 * @brief Sends a command to a Cisco device and waits for the response
 * 
 * This function sends a command string to the Cisco device via serial connection,
 * automatically appending a newline character. After sending the command, it
 * waits for the device to respond and return to the command prompt.
 * 
 * @param conn Pointer to the serial connection structure
 * @param command The command string to send (without newline)
 * @param timeout Maximum time to wait for response in seconds
 * 
 * @return 0 on success, -1 on error (write failure or timeout waiting for prompt)
 * 
 * @note The command string should not include a newline character as one will
 *       be automatically appended.
 */
int cisco_send_command(serial_conn_t *conn, const char *command, int timeout) {
    char cmd_buffer[MAX_LINE_LEN];
    
    // Format command with newline
    snprintf(cmd_buffer, sizeof(cmd_buffer), "%s\n", command);
    
    // Send command
    if (serial_write(conn, cmd_buffer) < 0) {
        return -1;
    }
    
    // Wait for response and prompt
    return cisco_wait_for_prompt(conn, timeout);
}

/**
 * @brief Initializes the flash filesystem on a Cisco device
 * 
 * This function waits for the "flash_init" prompt to appear from the Cisco device,
 * then sends the "flash_init" command to initialize the flash filesystem. This
 * is typically required before performing file operations on the device's flash memory.
 * 
 * @param conn Pointer to the serial connection structure
 * @param timeout Maximum time to wait for flash_init prompt in seconds
 * 
 * @return 0 on success, -1 on timeout or error
 * 
 * @note This function should be called before attempting to list or manipulate
 *       files in the flash filesystem.
 */
int cisco_init_flash(serial_conn_t *conn, int timeout) {
    char buffer[MAX_LINE_LEN];
    
    // Wait for the flash_init prompt
    while (timeout > 0) {
        int bytes_read = serial_read_until(conn, buffer, sizeof(buffer), "\n");
        if (bytes_read > 0) {
            if (strstr(buffer, "flash_init") != NULL) {
                break;
            }
        }
        usleep(1000000); // 1 second delay
        timeout--;
    }
    
    if (timeout <= 0) {
        return -1; // Timeout waiting for flash_init prompt
    }
    
    // Send flash_init command
    if (cisco_send_command(conn, "flash_init", timeout) < 0) {
        return -1;
    }
    
    return 0;
}

/**
 * @brief Retrieves a directory listing from a Cisco device
 * 
 * This function sends a "dir" command to the Cisco device to list the contents
 * of the specified directory. It parses the response to extract file information
 * including names, sizes, permissions, and types. The results are stored in a
 * linked list of file_entry_t structures.
 * 
 * @param conn Pointer to the serial connection structure
 * @param path The directory path to list (e.g., "flash:/", "flash:/config")
 * @param files Pointer to receive the head of the file entry linked list
 * @param timeout Maximum time to wait for response in seconds
 * 
 * @return Number of files found on success, -1 on error
 * 
 * @note The caller is responsible for freeing the allocated file_entry_t structures
 *       when they are no longer needed. The function handles special cases for
 *       "flash:/" paths and automatically determines file types based on permissions
 *       and filename extensions.
 * 
 * @warning This function allocates memory for each file entry found. Memory leaks
 *          will occur if the returned list is not properly freed.
 */
int cisco_get_directory_listing(serial_conn_t *conn, const char *path, file_entry_t **files, int timeout) {
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
    if (cisco_send_command(conn, command, timeout) < 0) {
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
        char permissions[16], date_str[64], filename[MAX_PATH_LEN];
        
        if (sscanf(line, "%d %s %d %*s %s %[^\n]", 
                   &file_num, permissions, &size, date_str, filename) >= 4) {
            
            // Create file entry
            file_entry_t *file = malloc(sizeof(file_entry_t));
            if (!file) continue;
            
            strncpy(file->name, filename, MAX_PATH_LEN - 1);
            file->name[MAX_PATH_LEN - 1] = '\0';
            
            // Build full path
            if (strcmp(path, "flash:/") == 0) {
                // For flash:/ paths, we know the prefix is exactly 7 characters
                size_t filename_len = strlen(filename);
                if (filename_len > MAX_PATH_LEN - 8) { // 8 = strlen("flash:/") + 1 for null terminator
                    // Truncate filename to fit
                    strncpy(file->full_path, "flash:/", MAX_PATH_LEN - 1);
                    strncat(file->full_path, filename, MAX_PATH_LEN - 8);
                    file->full_path[MAX_PATH_LEN - 1] = '\0';
                } else {
                    // Use strcpy and strcat instead of snprintf to avoid truncation warnings
                    strcpy(file->full_path, "flash:/");
                    strcat(file->full_path, filename);
                }
            } else {
                // Check if path + filename combination is too long
                size_t path_len = strlen(path);
                size_t filename_len = strlen(filename);
                if (path_len + filename_len + 2 > MAX_PATH_LEN) { // +2 for "/" and null terminator
                    // Truncate to fit
                    strncpy(file->full_path, path, MAX_PATH_LEN - 1);
                    file->full_path[MAX_PATH_LEN - 1] = '\0';
                    strncat(file->full_path, "/", MAX_PATH_LEN - strlen(file->full_path) - 1);
                    strncat(file->full_path, filename, MAX_PATH_LEN - strlen(file->full_path) - 1);
                    file->full_path[MAX_PATH_LEN - 1] = '\0';
                } else {
                    // Use strcpy and strcat instead of snprintf to avoid truncation warnings
                    strcpy(file->full_path, path);
                    strcat(file->full_path, "/");
                    strcat(file->full_path, filename);
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

/**
 * @brief Deletes a file from a Cisco device's flash memory
 * 
 * This function sends a "delete" command to remove a file from the Cisco device's
 * flash memory. It automatically handles the confirmation prompt that Cisco devices
 * typically display before deleting files.
 * 
 * @param conn Pointer to the serial connection structure
 * @param file_path The full path to the file to delete (e.g., "flash:/config.txt")
 * @param timeout Maximum time to wait for response in seconds
 * 
 * @return 0 on successful deletion, -1 on error or deletion failure
 * 
 * @note The function automatically responds "y" to the confirmation prompt.
 *       If the file doesn't exist or cannot be deleted, the function will return -1.
 * 
 * @warning This operation is irreversible. Ensure the file_path is correct
 *          before calling this function.
 */
int cisco_delete_file(serial_conn_t *conn, const char *file_path, int timeout) {
    char command[MAX_LINE_LEN];
    char buffer[MAX_LINE_LEN];
    
    // Build delete command
    snprintf(command, sizeof(command), "delete %s", file_path);
    
    // Send delete command
    if (cisco_send_command(conn, command, timeout) < 0) {
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

/**
 * @brief Deletes a directory from a Cisco device's flash memory
 * 
 * This function sends an "rmdir" command to remove a directory from the Cisco device's
 * flash memory. It automatically handles the confirmation prompt that Cisco devices
 * typically display before removing directories.
 * 
 * @param conn Pointer to the serial connection structure
 * @param dir_path The full path to the directory to delete (e.g., "flash:/backup")
 * @param timeout Maximum time to wait for response in seconds
 * 
 * @return 0 on successful removal, -1 on error or removal failure
 * 
 * @note The function automatically responds "y" to the confirmation prompt.
 *       The directory must be empty for the removal to succeed. If the directory
 *       doesn't exist or cannot be removed, the function will return -1.
 * 
 * @warning This operation is irreversible. Ensure the dir_path is correct and
 *          the directory is empty before calling this function.
 */
int cisco_delete_directory(serial_conn_t *conn, const char *dir_path, int timeout) {
    char command[MAX_LINE_LEN];
    char buffer[MAX_LINE_LEN];
    
    // Build rmdir command
    snprintf(command, sizeof(command), "rmdir %s", dir_path);
    
    // Send rmdir command
    if (cisco_send_command(conn, command, timeout) < 0) {
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