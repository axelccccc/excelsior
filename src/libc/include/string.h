/**
 * @file string.h
 * @author axelccccc (github.com/axelccccc)
 * @brief String manipulation functions
 * @version 0.1
 * @date 2022-12-10
 * 
 * Copyright (c) 2022 axelccccc
 * 
 */

#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Copy memory from one location to another
 * (non-overlapping)
 * @param src Source memory location
 * @param dest Destination memory location
 * @param count Number of bytes to copy
 * @return void* Destination memory location
 */
void* memcpy(void* restrict src, void* restrict dest, size_t count);

/**
 * @brief Set memory to a value
 * 
 * @param dest Destination memory location
 * @param val Value to set
 * @param count Number of bytes to set
 * @return void* Destination memory location
 */
void* memset(void* dest, char val, size_t count);

/**
 * @brief Compare two memory locations
 * 
 * @param ptr1 First memory location
 * @param ptr2 Second memory location
 * @param count Number of bytes to compare
 * @return int 0 if equal, 1 if ptr1 > ptr2, -1 if ptr1 < ptr2
 */
int memcmp(const void* ptr1, const void* ptr2, size_t count);

/**
 * @brief Copy memory from one location to another
 * with overlapping support
 * @param src Source memory location
 * @param dest Destination memory location
 * @param count Number of bytes to copy
 * @return void* Destination memory location
 */
void* memmove(void* src, void* dest, size_t count);

/**
 * @brief Get the length of a null-terminated string
 * 
 * @param str Null-terminated string
 * @return size_t Number of characters in string
 */
size_t strlen(const char* str);

#ifdef __cplusplus
}
#endif

#endif