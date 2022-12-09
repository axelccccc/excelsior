#pragma once

#include <stddef.h>

/**
 * @brief Copy memory from one location to another
 * 
 * @param src Source memory location
 * @param dest Destination memory location
 * @param count Number of bytes to copy
 */
void memcpy(char* src, char* dest, size_t count);

/**
 * @brief Set memory to a value
 * 
 * @param dest Destination memory location
 * @param val Value to set
 * @param count Number of bytes to set
 */
void memset(char* dest, char val, size_t count);