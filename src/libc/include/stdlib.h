/**
 * @file stdlib.h
 * @author axelccccc (github.com/axelccccc)
 * @brief ???
 * @version 0.1
 * @date 2022-12-13
 * 
 * Copyright (c) 2022 axelccccc
 * 
 */

#ifndef _STDLIB_H
#define _STDLIB_H 1

/**
 * @brief Convert the first number in 
 * a string to an integer (Base 10)
 * @param str String to convert 
 * @return int Converted integer
 */
int atoi(const char* str);

/**
 * @brief Convert an integer to a string
 * 
 * @param n Number to convert
 * @param str String to store the result
 * @param base Base to convert to
 * @return char* Pointer to the string
 * or NULL if a non-supported base was given
 */
char* itoa(int n, char* str, int base);

/**
 * @brief Abort the program
 */
__attribute__((noreturn))
void abort(void);

#endif