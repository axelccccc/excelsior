#include "term.h"

void term_scroll(size_t rows) {

    for(int i = rows; i < VGA_HEIGHT; i++) {
        for(int j = 0; j < VGA_WIDTH; j++) {
            terminal_buffer[i-rows + j] = terminal_buffer[i + j];
        }
    }
}

void term_move_cursor(size_t distance, enum CUR_DIR dir) {

    terminal_column += distance;
    terminal_row += terminal_column / VGA_WIDTH;
    terminal_column %= VGA_WIDTH;

    if(terminal_row >= VGA_HEIGHT) {
        size_t offset = terminal_row - VGA_HEIGHT;
        term_scroll(offset);
    }

}

void term_putchar(const char c) {

    // handle newline
    if(c == 10) {
        // term_move_cursor(VGA_WIDTH - terminal_column, CUR_DIR_FORWARD);
        terminal_column = 0; // faster than above
        terminal_row++;
    } else {
        terminal_buffer[terminal_row * VGA_WIDTH + terminal_column]
            = vga_entry(c, terminal_color);
        term_move_cursor(1, CUR_DIR_FORWARD);
    }
    
}

void term_print(const char* str) {
    while(*str != 0) {
        term_putchar(*str++);
    }
}

void term_clear(void) {
    terminal_row = 0;
    terminal_column = 0;
    int num_chars = VGA_WIDTH * VGA_HEIGHT;
    while(num_chars != 0) {
        term_putchar(' ');
        --num_chars;
    }
    terminal_row = 0;
    terminal_column = 0;
}

void term_init(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*)VIDEO_MEMORY;
}