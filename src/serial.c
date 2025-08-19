#include "cisco_cli.h"

int serial_init(serial_conn_t *conn, const char *port, int baud_rate) {
    struct termios tty;
    
    // Open serial port
    conn->fd = open(port, O_RDWR | O_NOCTTY | O_SYNC);
    if (conn->fd < 0) {
        return -1;
    }
    
    // Get current settings
    if (tcgetattr(conn->fd, &tty) != 0) {
        close(conn->fd);
        return -1;
    }
    
    // Set baud rate
    cfsetospeed(&tty, baud_rate);
    cfsetispeed(&tty, baud_rate);
    
    // 8N1 (8 data bits, no parity, 1 stop bit)
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag &= ~(PARENB | PARODD);
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;
    
    // Raw input
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    
    // Raw output
    tty.c_oflag &= ~OPOST;
    
    // Set local mode
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    
    // Set timeout
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 5;
    
    // Apply settings
    if (tcsetattr(conn->fd, TCSANOW, &tty) != 0) {
        close(conn->fd);
        return -1;
    }
    
    // Initialize mutex
    pthread_mutex_init(&conn->mutex, NULL);
    
    // Store settings
    strncpy(conn->port, port, MAX_PATH_LEN - 1);
    conn->port[MAX_PATH_LEN - 1] = '\0';
    conn->baud_rate = baud_rate;
    conn->buffer_pos = 0;
    conn->debug_enabled = 1;  // Enable debug by default
    
    return 0;
}

// Function to control debug output
void serial_set_debug(serial_conn_t *conn, int enabled) {
    conn->debug_enabled = enabled;
}

int serial_write(serial_conn_t *conn, const char *data) {
    pthread_mutex_lock(&conn->mutex);
    
    int len = strlen(data);
    int written = write(conn->fd, data, len);
    
    // Also write to stdout for debugging (only if debug is enabled)
    if (conn->debug_enabled) {
        printf("SENT: %s", data);
    }
    
    pthread_mutex_unlock(&conn->mutex);
    return written;
}

int serial_read(serial_conn_t *conn, char *buffer, int max_len) {
    pthread_mutex_lock(&conn->mutex);
    
    int bytes_read = read(conn->fd, buffer, max_len - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        
        // Also print to stdout for debugging (only if debug is enabled)
        if (conn->debug_enabled) {
            printf("RECV: %s", buffer);
        }
    }
    
    pthread_mutex_unlock(&conn->mutex);
    return bytes_read;
}

int serial_read_until(serial_conn_t *conn, char *buffer, int max_len, const char *delimiter) {
    int total_read = 0;
    char temp_buffer[256];
    char *delim_pos;
    
    buffer[0] = '\0';
    
    while (total_read < max_len - 1) {
        int bytes_read = serial_read(conn, temp_buffer, sizeof(temp_buffer) - 1);
        if (bytes_read <= 0) {
            usleep(10000); // 10ms delay
            continue;
        }
        
        // Append to main buffer
        strncat(buffer, temp_buffer, max_len - total_read - 1);
        total_read += bytes_read;
        
        // Check for delimiter
        delim_pos = strstr(buffer, delimiter);
        if (delim_pos) {
            // Include the delimiter in the result
            delim_pos += strlen(delimiter);
            *delim_pos = '\0';
            return strlen(buffer);
        }
    }
    
    return total_read;
}

void serial_close(serial_conn_t *conn) {
    if (conn->fd >= 0) {
        close(conn->fd);
        conn->fd = -1;
    }
    pthread_mutex_destroy(&conn->mutex);
} 