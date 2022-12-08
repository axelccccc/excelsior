#pragma once

#include <stddef.h>
#include <stdint.h>

#define VIDEO_MEMORY 0xB8000
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define DEFAULT_TERM_COLOR 0x07 // vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK)

static uint16_t* terminal_buffer;

/* Hardware text mode color constants. */
enum vga_color {
    VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

/**
 * @brief Format VGA character attributes
 * 
 * @param fg Foreground color (font color)
 * @param bg Background color
 * @return uint8_t VGA-formatted character attributes
 */
static inline uint8_t vga_entry_color(
    enum vga_color fg,
    enum vga_color bg) {
        return fg | (bg << 4);
}

/**
 * @brief Format a character and its attributes
 * as a video-buffer compatible terminal character
 * @param uc Character
 * @param color VGA color (see `vga_entry_color()`)
 * @return uint16_t VGA-formatted character
 */
static inline uint16_t vga_entry(
    unsigned char uc,
    uint8_t color) {
        return (uint16_t)uc | ((uint16_t)color << 8);
}

/**
 * @brief Get current cursor position in video buffer
 * 
 * @return uint16_t Cursor position in video buffer
 */
uint16_t term_get_cursor_offset();

/**
 * @brief Set cursor position in video buffer
 * 
 * @param offset Cursor position in video buffer
 */
void term_set_cursor_offset(uint16_t offset);

/**
 * @brief Get current cursor position in terminal
 * 
 * @param row Row number
 * @param column Column number
 */
void term_get_cursor_pos(size_t* row, size_t* column);

/**
 * @brief Set cursor position in terminal
 * 
 * @param row Row number
 * @param column Column number
 */
void term_set_cursor_pos(size_t row, size_t column);

/**
 * @brief Moves each row of characters up `rows` rows
 * 
 * @param rows Number of rows to scroll up
 * @note with this routine, once text disappears
 *       from the top of the screen, it is not
 *       stored and so definitely lost.
 * @todo implement stored string buffer
 * @todo implement bidirectional scrolling
 */
void term_scroll(size_t rows);

/**
 * @brief Moves current character position by some distance
 * forwards or backwards. It wraps on end of line and scrolls
 * on end of screen
 * @param distance Distance to move in number of characters
 */
void term_move_cursor(size_t distance);

/**
 * @brief Print a character on terminal
 * 
 * @param c Character to print
 */
void term_putchar(const char c);

/**
 * @brief Print a null-terminated string on terminal
 * 
 * @param str Null-terminated string
 */
void term_print(const char* str);

/**
 * @brief Clears terminal screen
 * 
 */
void term_clear(void);

/**
 * @brief Initialize terminal and video buffer
 * 
 */
void term_init(void);