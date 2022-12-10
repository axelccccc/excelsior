/**
 * @file memcpy.c
 * @author axelccccc (github.com/axelccccc)
 * @brief Copy memory blocks
 * @version 0.1
 * @date 2022-12-10
 * 
 * Copyright (c) 2022 axelccccc
 * 
 */

#include <string.h>

void* memcpy(void* restrict src, void* restrict dest, size_t count) {

    unsigned char* src8 = (unsigned char*)src;
    unsigned char* dest8 = (unsigned char*)dest;

    for(size_t i = 0; i < count; i++)
        dest8[i] = src8[i];
    
    return dest;
    
}