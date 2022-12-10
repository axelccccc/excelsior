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

void* memset(void* dest, char val, size_t count) {
    
    unsigned char* dest8 = (unsigned char*)dest;
    
    for(size_t i = 0; i < count; i++) {
        dest8[i] = val;
    }

    return dest;

}