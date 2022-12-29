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

#include <stddef.h>

/**********************/
/***** CONVERSION *****/
/**********************/

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



/**********************/
/******* SYSTEM *******/
/**********************/

/**
 * @brief Abort the program
 */
__attribute__((noreturn))
void abort(void);



/**********************/
/******* MEMORY *******/
/**********************/

/**
 * @brief Dynamically allocate memory
 * on the heap
 * @param size Size of the memory to allocate (in bytes) 
 * @return void* Pointer to the allocated memory
 */
void* malloc(size_t size);

/**
 * @brief Free memory allocated with malloc
 * on the heap
 * @param ptr Pointer to the memory to free
 */
void free(void* ptr);



/***********************/
/***** ALGORITHMIC *****/
/***********************/

/**
 * @brief Sort an array of elements
 * (using the quicksort algorithm)
 * @param base Pointer to the array
 * @param num Number of elements in the array
 * @param size Size of each element (in bytes)
 * @param cmp Comparison function returning:
 *            < 0 if a < b;
 *            0 if a == b;
 *            > 0 if a > b;
 */
void qsort(void* base, size_t num, size_t size, int (*cmp)(const void*, const void*));

#endif