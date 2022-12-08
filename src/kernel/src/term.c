#include "term.h"
#include "asm.h"

void term_get_cursor_pos(size_t* row, size_t* column) {
    // reg 14: high byte of cursor offset
    port_byte_out(REG_SCREEN_CTRL, 14);
    uint32_t offset = port_byte_in(REG_SCREEN_DATA) << 8;
    // reg 15: low byte of cursor offset
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    *row = offset / VGA_WIDTH;
    *column = offset % VGA_WIDTH;
}

void term_set_cursor_pos(size_t row, size_t column) {
    uint32_t offset = row * VGA_WIDTH + column;
    // reg 14: high byte of cursor offset
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, offset >> 8);
    // reg 15: low byte of cursor offset
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, offset);
}

uint16_t term_get_cursor_offset() {
    // reg 14: high byte of cursor offset
    port_byte_out(REG_SCREEN_CTRL, 14);
    uint32_t offset = port_byte_in(REG_SCREEN_DATA) << 8;
    // reg 15: low byte of cursor offset
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset;
}

void term_set_cursor_offset(uint16_t offset) {
    // reg 14: high byte of cursor offset
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, offset >> 8);
    // reg 15: low byte of cursor offset
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, offset);
}

void term_move_cursor(size_t distance) {

    uint16_t offset = term_get_cursor_offset();

    offset += distance;

    term_set_cursor_offset(offset);

    // scroll if cursor gets out of screen
    if(offset >= VGA_WIDTH * VGA_HEIGHT) {
        uint16_t rows = ((offset - (VGA_WIDTH * VGA_HEIGHT)) / VGA_WIDTH) + 1;
        term_scroll(rows);
    }

}

void term_putchar(const char c) {

    // get cursor position
    size_t row = 0, column = 0;
    term_get_cursor_pos(&row, &column);

    if(c == 10) { // handle newline
        term_move_cursor(VGA_WIDTH - column);
    } else {
        terminal_buffer[row * VGA_WIDTH + column]
            = vga_entry(c, DEFAULT_TERM_COLOR);
        term_move_cursor(1);
    }
    
}

void term_scroll(size_t rows) {

    // move all rows up by `rows`
    for(int i = rows; i < VGA_HEIGHT; i++) {
        for(int j = 0; j < VGA_WIDTH; j++) {
            terminal_buffer[(i-rows)*VGA_WIDTH + j] = terminal_buffer[i*VGA_WIDTH + j];
        }
    }

    // clear the last `rows` rows
    for(int i = VGA_HEIGHT - rows; i < VGA_HEIGHT; i++) {
        for(int j = 0; j < VGA_WIDTH; j++) {
            terminal_buffer[i*VGA_WIDTH + j] = vga_entry(' ', DEFAULT_TERM_COLOR);
        }
    }

    // move cursor up by `rows`
    term_set_cursor_offset(term_get_cursor_offset() - rows * VGA_WIDTH);

}

void term_print(const char* str) {
    while(*str != 0) {
        term_putchar(*str++);
    }
}

void term_clear(void) {
    int num_chars = VGA_WIDTH * VGA_HEIGHT;
    for(int i = 0; i < num_chars; i++) {
        terminal_buffer[i] = vga_entry(' ', 0x07);
    }
    term_set_cursor_offset(0);
}

void term_init(void) {
    terminal_buffer = (uint16_t*)VIDEO_MEMORY;
}