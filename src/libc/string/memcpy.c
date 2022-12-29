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
#include <stdint.h>

void* memcpy(void* restrict dst, void* restrict src, size_t count) {

    uint8_t* src8 = (uint8_t*)src;
    uint8_t* dst8 = (uint8_t*)dst;

    for(size_t i = 0; i < count; i++)
        dst8[i] = src8[i];
    
    return dst;
    
}