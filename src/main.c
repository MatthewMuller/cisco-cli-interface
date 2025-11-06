#include "cisco_cli.h"

// Utility functions
file_type_t get_file_type(const char *filename) {
  if (is_binary_file(filename)) {
    return FILE_TYPE_BINARY;
  }

  // Check if it's a directory (this would need to be determined from the dir
  // listing) For now, we'll assume it's a regular file
  return FILE_TYPE_REGULAR;
}

int is_binary_file(const char *filename) {
  const char *ext = strrchr(filename, '.');
  if (!ext)
    return 0;

  return (strcasecmp(ext, ".bin") == 0 || strcasecmp(ext, ".tar") == 0 ||
          strcasecmp(ext, ".pkg") == 0);
}

void trim_whitespace(char *str) {
  char *end;

  // Trim leading space
  while (isspace((unsigned char)*str))
    str++;

  if (*str == 0)
    return; // All spaces

  // Trim trailing space
  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end))
    end--;

  // Write new null terminator
  end[1] = '\0';
}

char *strdup_safe(const char *str) {
  if (!str)
    return NULL;
  return strdup(str);
}

void print_usage(const char *program_name) {
  printf("Usage: %s <serial_port> [baud_rate]\n", program_name);
  printf("  serial_port: Serial port (e.g., /dev/ttyUSB0)\n");
  printf("  baud_rate:   Baud rate (default: %d)\n", DEFAULT_BAUD_RATE);
  printf("\nExample: %s /dev/ttyUSB0 %d\n", program_name, DEFAULT_BAUD_RATE);
}

int main(int argc, char *argv[]) {
  app_state_t app = {0};
  char serial_port[MAX_PATH_LEN] = "/dev/ttyUSB0";
  int baud_rate = DEFAULT_BAUD_RATE;

  // Parse command line arguments
  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  strncpy(serial_port, argv[1], MAX_PATH_LEN - 1);
  serial_port[MAX_PATH_LEN - 1] = '\0';

  if (argc >= 3) {
    baud_rate = atoi(argv[2]);
    switch (baud_rate) {
    case 9600:
      baud_rate = B9600;
      break;
    case 19200:
      baud_rate = B19200;
      break;
    case 38400:
      baud_rate = B38400;
      break;
    case 57600:
      baud_rate = B57600;
      break;
    case 115200:
      baud_rate = B115200;
      break;
    default:
      printf("Invalid baud rate. Using %d.\n", DEFAULT_BAUD_RATE);
      baud_rate = DEFAULT_BAUD_RATE;
      break;
    }
  }

  // Initialize UI
  ui_init();

  // Initialize serial connection
  printf("Connecting to %s at %d baud...\n", serial_port, baud_rate);
  if (serial_init(&app.serial, serial_port, baud_rate) < 0) {
    ui_cleanup();
    printf("Failed to open serial port %s\n", serial_port);
    return 1;
  }

  app.connected = 1;
  ui_draw_status(&app, "Connected - Waiting for switch to boot...");

  // Wait for switch to boot and initialize flash
  printf("Waiting for switch to boot and initialize flash...\n");
  if (cisco_init_flash(&app.serial, FLASH_INIT_TIMEOUT) < 0) {
    ui_cleanup();
    serial_close(&app.serial);
    printf("Failed to initialize flash on switch\n");
    return 1;
  }

  app.flash_initialized = 1;
  ui_draw_status(&app, "Connected - Flash Ready");

  // Disable debug output now that UI is active
  serial_set_debug(&app.serial, 0);

  // Initialize UI state
  app.ui.current_page = 0;
  app.ui.selected_item = 0;
  app.ui.max_items = 5;
  app.ui.scroll_pos = 0;

  // Main application loop
  int running = 1;
  while (running) {
    switch (app.ui.current_page) {
    case 0: // Main menu
      ui_draw_main_menu(&app);
      break;

    case 1: // Tree view
      ui_draw_tree_view(&app);
      break;

    case 2: // Selection view
      ui_draw_selection_view(&app);
      break;
    }

    int input_result = ui_handle_input(&app);

    switch (input_result) {
    case -1: // Quit
      running = 0;
      break;

    case 1: // View Flash Directory Tree
      app.ui.current_page = 1;
      app.ui.selected_item = 0;
      app.ui.scroll_pos = 0;

      // Build file tree if not already built
      if (!app.file_tree) {
        ui_draw_status(&app, "Building file tree...");
        int result = file_tree_build(&app.serial, &app.file_tree);
        if (result == 0) {
          ui_draw_status(&app, "File tree built");
        } else {
          ui_draw_status(&app, "Failed to build file tree");
        }
      }
      break;

    case 2: // Select Files/Folders to Delete
      app.ui.current_page = 2;
      app.ui.selected_item = 0;
      app.ui.scroll_pos = 0;

      // Build file tree if not already built
      if (!app.file_tree) {
        ui_draw_status(&app, "Building file tree...");
        int result = file_tree_build(&app.serial, &app.file_tree);
        if (result == 0) {
          ui_draw_status(&app, "File tree built");
        } else {
          ui_draw_status(&app, "Failed to build file tree");
        }
      }
      break;

    case 3: // Delete Selected Items
      if (app.file_tree) {
        int selected_count = 0;
        file_tree_count_selected(app.file_tree, &selected_count);
        if (selected_count > 0) {
          char confirm_msg[256];
          snprintf(confirm_msg, sizeof(confirm_msg),
                   "Delete %d selected items?", selected_count);

          int confirmed = 0;
          ui_show_confirm_dialog(confirm_msg, &confirmed);

          if (confirmed) {
            ui_draw_status(&app, "Deleting selected items...");
            int success_count = 0, fail_count = 0;
            int result = file_tree_delete_selected(&app.serial, app.file_tree,
                                                   &success_count, &fail_count);
            if (result == 0) {
              char status_msg[256];
              snprintf(status_msg, sizeof(status_msg),
                       "Deletion complete: %d successful, %d failed",
                       success_count, fail_count);
              ui_draw_status(&app, status_msg);
            } else {
              ui_draw_status(&app, "Deletion failed");
            }

            // Refresh file tree
            file_tree_free(app.file_tree);
            app.file_tree = NULL;
          }
        } else {
          ui_draw_status(&app, "No items selected for deletion");
        }
      }
      break;

    case 4: // Refresh File Tree
      if (app.file_tree) {
        file_tree_free(app.file_tree);
        app.file_tree = NULL;
      }
      ui_draw_status(&app, "Refreshing file tree...");
      int result = file_tree_build(&app.serial, &app.file_tree);
      if (result == 0) {
        ui_draw_status(&app, "File tree refreshed");
      } else {
        ui_draw_status(&app, "Failed to refresh file tree");
      }
      break;

    case 5: // Exit
      running = 0;
      break;
    }
  }

  // Cleanup
  if (app.file_tree) {
    file_tree_free(app.file_tree);
  }

  serial_close(&app.serial);
  ui_cleanup();

  printf("Cisco CLI Interface closed.\n");
  return 0;
}
