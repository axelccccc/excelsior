#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// #if defined(__linux__)
// #error "Wrong OS target"
// #endif

#if !defined(__i386__)
#error "This kernel must be compiled for an ix86 target (-m elf_i386)"
#endif

static const uint32_t VIDEO_MEMORY = 0xB8000;

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

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

/* Cursor direction */
enum CUR_DIR {
    CUR_DIR_BACKWARD,
    CUR_DIR_FORWARD
};

/* ***** PROTOTYPES ***** */

static inline uint8_t vga_entry_color(
    enum vga_color fg,
    enum vga_color bg);
static inline uint16_t vga_entry(
    unsigned char uc,
    uint8_t color);

size_t strlen(const char* str);

void term_scroll(size_t rows);
void term_move_cursor(size_t distance, enum CUR_DIR dir);
void term_putchar(const char c);
void term_print(const char* str);
void term_init(void);

void main(void) __attribute__((section (".text.main")));

/* ********************** */

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
 * @brief Get the length of a null-terminated string
 * 
 * @param str Null-terminated string
 * @return size_t Number of characters in string
 */
size_t strlen(const char* str) {
    size_t len = 0;
    while(str[len])
        len++;
    return len;
}

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
void term_scroll(size_t rows) {

    for(int i = rows; i < VGA_HEIGHT; i++) {
        for(int j = 0; j < VGA_WIDTH; j++) {
            terminal_buffer[i-rows + j] = terminal_buffer[i + j];
        }
    }
}

/**
 * @brief Moves current character position by some distance
 * forwards or backwards. It wraps on end of line and scrolls
 * on end of screen
 * @param distance Distance to move in number of characters
 * @param dir Direction in which to move (back/forwards)
 */
void term_move_cursor(size_t distance, enum CUR_DIR dir) {

    terminal_column += distance;
    terminal_row += terminal_column / VGA_WIDTH;
    terminal_column %= VGA_WIDTH;

    if(terminal_row >= VGA_HEIGHT) {
        size_t offset = terminal_row - VGA_HEIGHT;
        term_scroll(offset);
    }

}

/**
 * @brief Print a character on terminal
 * 
 * @param c Character to print
 */
void term_putchar(const char c) {

    // handle newline
    if(c == 10) {
        term_move_cursor(VGA_WIDTH - terminal_column, CUR_DIR_FORWARD);
    } else {
        terminal_buffer[terminal_row * VGA_WIDTH + terminal_column]
            = vga_entry(c, terminal_color);
        term_move_cursor(1, CUR_DIR_FORWARD);
    }
    
}

/**
 * @brief Print a null-terminated string on terminal
 * 
 * @param str Null-terminated string
 */
void term_print(const char* str) {
    while(*str != 0) {
        term_putchar(*str++);
    }
}

/**
 * @brief Clears temrinal screen
 * 
 */
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

void main(void) {

    term_init();
    term_clear();
    term_print("Welcome to my OS!\nTest another line\nAnd another");
    while(true) {}
    
}