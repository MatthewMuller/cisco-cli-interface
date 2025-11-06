#ifndef CISCO_CLI_H
#define CISCO_CLI_H

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h>
#include <ncurses.h>
#include <ctype.h>
#include <time.h>

// Define CRTSCTS if not available
#ifndef CRTSCTS
#define CRTSCTS 020000000000
#endif

// Constants
#define MAX_PATH_LEN 256
#define MAX_LINE_LEN 1024
#define MAX_FILES 1000
#define MAX_DIRS 100
#define SERIAL_BUFFER_SIZE 4096

// Default configuration values
#define DEFAULT_BAUD_RATE 9600
#define DEFAULT_TIMEOUT 30
#define MAX_BAUD_RATES 5
#define FLASH_INIT_TIMEOUT 6
#define COMMAND_TIMEOUT 3
#define DELETE_TIMEOUT 3

// Error codes for file tree functions
#define FILE_TREE_SUCCESS 0
#define FILE_TREE_ERROR_NULL_PARAM -1
#define FILE_TREE_ERROR_MEMORY_ALLOC -2
#define FILE_TREE_ERROR_INVALID_PARAM -3
#define FILE_TREE_ERROR_OPERATION_FAILED -4

// File types
typedef enum {
    FILE_TYPE_REGULAR,
    FILE_TYPE_DIRECTORY,
    FILE_TYPE_BINARY
} file_type_t;

// File entry structure
typedef struct file_entry {
    char name[MAX_PATH_LEN];
    char full_path[MAX_PATH_LEN];
    file_type_t type;
    long size;
    time_t date;
    int selected;
    struct file_entry *next;
} file_entry_t;

// Directory tree structure
typedef struct dir_node {
    char name[MAX_PATH_LEN];
    char path[MAX_PATH_LEN];
    file_type_t type;
    int selected;
    int expanded;
    int level;
    struct dir_node *parent;
    struct dir_node *children;
    struct dir_node *next;
} dir_node_t;

// Serial connection structure
typedef struct {
    int fd;
    char port[MAX_PATH_LEN];
    int baud_rate;
    pthread_mutex_t mutex;
    char buffer[SERIAL_BUFFER_SIZE];
    int buffer_pos;
    int debug_enabled;  // Flag to control debug output
} serial_conn_t;

// UI state structure
typedef struct {
    WINDOW *main_win;
    WINDOW *status_win;
    WINDOW *menu_win;
    int current_page;
    int max_pages;
    int scroll_pos;
    int selected_item;
    int max_items;
} ui_state_t;

// Application state
typedef struct {
    serial_conn_t serial;
    ui_state_t ui;
    dir_node_t *file_tree;
    int connected;
    int flash_initialized;
} app_state_t;

// Function declarations

// Serial communication
int serial_init(serial_conn_t *conn, const char *port, int baud_rate);
int serial_write(serial_conn_t *conn, const char *data);
int serial_read(serial_conn_t *conn, char *buffer, int max_len);
int serial_read_until(serial_conn_t *conn, char *buffer, int max_len, const char *delimiter);
void serial_set_debug(serial_conn_t *conn, int enabled);
void serial_close(serial_conn_t *conn);

// Cisco commands
// Note: All timeout parameters are in seconds
int cisco_wait_for_prompt(serial_conn_t *conn, int timeout);
int cisco_send_command(serial_conn_t *conn, const char *command, int timeout);
int cisco_init_flash(serial_conn_t *conn, int timeout);
int cisco_get_directory_listing(serial_conn_t *conn, const char *path, file_entry_t **files, int timeout);
int cisco_delete_file(serial_conn_t *conn, const char *file_path, int timeout);
int cisco_delete_directory(serial_conn_t *conn, const char *dir_path, int timeout);

// File tree management
int file_tree_create(const char *name, const char *path, file_type_t type, dir_node_t **node);
int file_tree_add_child(dir_node_t *parent, dir_node_t *child);
int file_tree_build_recursive(serial_conn_t *conn, dir_node_t *parent, const char *path);
int file_tree_build(serial_conn_t *conn, dir_node_t **root);
int file_tree_free(dir_node_t *node);
int file_tree_select(dir_node_t *node, int selected);
int file_tree_count_selected(dir_node_t *node, int *count);
int file_tree_delete_selected_recursive(serial_conn_t *conn, dir_node_t *node, int *success_count, int *fail_count);
int file_tree_delete_selected(serial_conn_t *conn, dir_node_t *node, int *success_count, int *fail_count);
int file_tree_get_flat_list(dir_node_t *node, dir_node_t **list, int max_count, int *count);

// UI functions
void ui_init(void);
void ui_cleanup(void);
void ui_draw_main_menu(app_state_t *app);
void ui_draw_tree_view(app_state_t *app);
void ui_draw_selection_view(app_state_t *app);
void ui_draw_status(app_state_t *app, const char *message);
int ui_handle_input(app_state_t *app);
void ui_show_confirm_dialog(const char *message, int *confirmed);

// Utility functions
file_type_t get_file_type(const char *filename);
int is_binary_file(const char *filename);
void trim_whitespace(char *str);
char *strdup_safe(const char *str);

#endif // CISCO_CLI_H
