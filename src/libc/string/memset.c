/**
 * @file memset.c
 * @author axelccccc (github.com/axelccccc)
 * @brief Fill memory blocks
 * @version 0.1
 * @date 2022-12-10
 * 
 * Copyright (c) 2022 axelccccc
 * 
 */

#include <string.h>

void* memset(void* dst, char val, size_t count) {
    
    unsigned char* dst8 = (unsigned char*)dst;
    
    for(size_t i = 0; i < count; i++) {
        dst8[i] = val;
    }

    return dst;

}