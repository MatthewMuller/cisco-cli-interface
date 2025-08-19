#include "cisco_cli.h"

void ui_init(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    
    // Enable colors if available
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLUE);   // Selected item
        init_pair(2, COLOR_YELLOW, COLOR_BLACK); // Directory
        init_pair(3, COLOR_RED, COLOR_BLACK);    // Binary file
        init_pair(4, COLOR_GREEN, COLOR_BLACK);  // Regular file
        init_pair(5, COLOR_CYAN, COLOR_BLACK);   // Status
    }
}

void ui_cleanup(void) {
    endwin();
}

void ui_draw_main_menu(app_state_t *app) {
    clear();
    
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    // Draw title
    attron(A_BOLD);
    mvprintw(1, (max_x - 25) / 2, "Cisco CLI Interface");
    attroff(A_BOLD);
    
    // Draw menu options
    int start_y = max_y / 2 - 2;
    const char *options[] = {
        "1. View Flash Directory Tree",
        "2. Select Files/Folders to Delete",
        "3. Delete Selected Items",
        "4. Refresh File Tree",
        "5. Exit"
    };
    
    int num_options = sizeof(options) / sizeof(options[0]);
    
    for (int i = 0; i < num_options; i++) {
        int y = start_y + i;
        if (y < max_y - 2) {
            if (i == app->ui.selected_item) {
                attron(A_REVERSE);
            }
            mvprintw(y, (max_x - strlen(options[i])) / 2, "%s", options[i]);
            if (i == app->ui.selected_item) {
                attroff(A_REVERSE);
            }
        }
    }
    
    // Draw status
    attron(COLOR_PAIR(5));
    mvprintw(max_y - 2, 1, "Status: %s", 
             app->connected ? (app->flash_initialized ? "Connected - Flash Ready" : "Connected - Initializing Flash...") : "Not Connected");
    attroff(COLOR_PAIR(5));
    
    // Draw instructions
    mvprintw(max_y - 1, 1, "Use arrow keys to navigate, Enter to select, q to quit");
    
    refresh();
}

void ui_draw_tree_view(app_state_t *app) {
    clear();
    
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    // Draw title
    attron(A_BOLD);
    mvprintw(0, 1, "Flash Directory Tree");
    attroff(A_BOLD);
    
    if (!app->file_tree) {
        mvprintw(max_y / 2, (max_x - 20) / 2, "No file tree available");
        mvprintw(max_y - 1, 1, "Press 'r' to refresh, 'q' to quit");
        refresh();
        return;
    }
    
    // Get flat list of visible nodes
    dir_node_t *node_list[MAX_FILES];
    int node_count = file_tree_get_flat_list(app->file_tree, node_list, MAX_FILES);
    
    // Calculate display range
    int display_height = max_y - 4;
    int start_idx = app->ui.scroll_pos;
    int end_idx = start_idx + display_height;
    if (end_idx > node_count) end_idx = node_count;
    
    // Draw file tree
    int y = 2;
    for (int i = start_idx; i < end_idx && y < max_y - 2; i++) {
        dir_node_t *node = node_list[i];
        
        // Indent based on level
        int indent = node->level * 2;
        if (indent >= max_x - 20) indent = max_x - 20;
        
        // Draw indentation
        mvprintw(y, 1, "%*s", indent, "");
        
        // Draw expand/collapse indicator for directories
        if (node->type == FILE_TYPE_DIRECTORY) {
            mvprintw(y, indent + 1, "%s", node->expanded ? "[-]" : "[+]");
            attron(COLOR_PAIR(2));
        } else if (node->type == FILE_TYPE_BINARY) {
            attron(COLOR_PAIR(3));
        } else {
            attron(COLOR_PAIR(4));
        }
        
        // Draw filename
        mvprintw(y, indent + 5, "%s", node->name);
        attroff(COLOR_PAIR(2) | COLOR_PAIR(3) | COLOR_PAIR(4));
        
        y++;
    }
    
    // Draw scroll indicator
    if (node_count > display_height) {
        int scroll_percent = (start_idx * 100) / (node_count - display_height);
        mvprintw(max_y - 2, 1, "Scroll: %d%% (%d/%d)", scroll_percent, start_idx + 1, node_count);
    }
    
    // Draw instructions
    mvprintw(max_y - 1, 1, "Arrow keys: navigate, Space: expand/collapse, r: refresh, q: quit");
    
    refresh();
}

void ui_draw_selection_view(app_state_t *app) {
    clear();
    
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    // Draw title
    attron(A_BOLD);
    mvprintw(0, 1, "Select Files/Folders to Delete");
    attroff(A_BOLD);
    
    if (!app->file_tree) {
        mvprintw(max_y / 2, (max_x - 20) / 2, "No file tree available");
        mvprintw(max_y - 1, 1, "Press 'r' to refresh, 'q' to quit");
        refresh();
        return;
    }
    
    // Get flat list of visible nodes
    dir_node_t *node_list[MAX_FILES];
    int node_count = file_tree_get_flat_list(app->file_tree, node_list, MAX_FILES);
    
    // Calculate display range
    int display_height = max_y - 6;
    int start_idx = app->ui.scroll_pos;
    int end_idx = start_idx + display_height;
    if (end_idx > node_count) end_idx = node_count;
    
    // Draw file tree with selection indicators
    int y = 2;
    for (int i = start_idx; i < end_idx && y < max_y - 4; i++) {
        dir_node_t *node = node_list[i];
        
        // Indent based on level
        int indent = node->level * 2;
        if (indent >= max_x - 20) indent = max_x - 20;
        
        // Draw indentation
        mvprintw(y, 1, "%*s", indent, "");
        
        // Draw selection indicator
        mvprintw(y, indent + 1, "[%c]", node->selected ? 'X' : ' ');
        
        // Draw expand/collapse indicator for directories
        if (node->type == FILE_TYPE_DIRECTORY) {
            mvprintw(y, indent + 5, "%s", node->expanded ? "[-]" : "[+]");
            attron(COLOR_PAIR(2));
        } else if (node->type == FILE_TYPE_BINARY) {
            attron(COLOR_PAIR(3));
        } else {
            attron(COLOR_PAIR(4));
        }
        
        // Draw filename
        mvprintw(y, indent + 9, "%s", node->name);
        attroff(COLOR_PAIR(2) | COLOR_PAIR(3) | COLOR_PAIR(4));
        
        // Highlight selected item
        if (i == app->ui.selected_item) {
            attron(A_REVERSE);
            mvchgat(y, 1, max_x - 2, A_REVERSE, 0, NULL);
            attroff(A_REVERSE);
        }
        
        y++;
    }
    
    // Draw selection count
    int selected_count = file_tree_count_selected(app->file_tree);
    mvprintw(max_y - 3, 1, "Selected: %d items", selected_count);
    
    // Draw instructions
    mvprintw(max_y - 2, 1, "Space: select/deselect, Arrow keys: navigate, d: delete selected, q: quit");
    
    refresh();
}

void ui_draw_status(app_state_t *app, const char *message) {
    (void)app; // Suppress unused parameter warning
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    attron(COLOR_PAIR(5));
    mvprintw(max_y - 2, 1, "Status: %s", message);
    attroff(COLOR_PAIR(5));
    refresh();
}

int ui_handle_input(app_state_t *app) {
    int ch = getch();
    
    switch (ch) {
        case 'q':
        case 'Q':
        case 27: // ESC
            return -1; // Quit
            
        case KEY_UP:
            if (app->ui.selected_item > 0) {
                app->ui.selected_item--;
            }
            break;
            
        case KEY_DOWN:
            if (app->ui.selected_item < app->ui.max_items - 1) {
                app->ui.selected_item++;
            }
            break;
            
        case ' ':
            // Handle space key based on current page
            if (app->ui.current_page == 1) { // Tree view
                // Toggle expand/collapse
                // This would need to be implemented with the actual node selection
                break;
            } else if (app->ui.current_page == 2) { // Selection view
                // Toggle selection
                // This would need to be implemented with the actual node selection
                break;
            }
            break;
            
        case 'r':
        case 'R':
            // Refresh
            return 1;
            
        case 'd':
        case 'D':
            // Delete selected
            if (app->ui.current_page == 2) {
                return 2;
            }
            break;
            
        case 10: // Enter
            return app->ui.selected_item + 1;
    }
    
    return 0;
}

void ui_show_confirm_dialog(const char *message, int *confirmed) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    // Create dialog window
    int dialog_height = 5;
    int dialog_width = strlen(message) + 10;
    int start_y = (max_y - dialog_height) / 2;
    int start_x = (max_x - dialog_width) / 2;
    
    WINDOW *dialog = newwin(dialog_height, dialog_width, start_y, start_x);
    box(dialog, 0, 0);
    
    // Draw message
    mvwprintw(dialog, 1, 2, "%s", message);
    mvwprintw(dialog, 3, 2, "Press 'y' to confirm, 'n' to cancel");
    
    wrefresh(dialog);
    
    // Wait for input
    int ch;
    while ((ch = wgetch(dialog)) != 'y' && ch != 'Y' && ch != 'n' && ch != 'N') {
        // Wait for valid input
    }
    
    *confirmed = (ch == 'y' || ch == 'Y');
    
    delwin(dialog);
    refresh();
} 