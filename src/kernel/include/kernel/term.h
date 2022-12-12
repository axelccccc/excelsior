#ifndef _KERNEL_TERM_H
#define _KERNEL_TERM_H

#include <stddef.h>
#include <stdint.h>

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

#endif