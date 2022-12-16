/**
 * @file stdio.h
 * @author axelccccc (github.com/axelccccc)
 * @brief Standard input/output functions
 * @version 0.1
 * @date 2022-12-12
 * 
 * Copyright (c) 2022 axelccccc
 * 
 */

#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Print a character to the screen
 * 
 * @param c Character to print
 */
void putchar(char c);

/**
 * @brief Print a string to the screen
 * 
 * @param str String to print
 * @return int Number of characters printed
 */
int puts(const char *str);

/**
 * @brief Print a formatted string to the screen
 * 
 * @param fmt Format string
 * @param ... Arguments
 * @return int
 */
int printf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif