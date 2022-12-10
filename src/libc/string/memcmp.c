/**
 * @file memcmp.c
 * @author axelccccc (github.com/axelccccc)
 * @brief Compare two memory blocks
 * @version 0.1
 * @date 2022-12-10
 * 
 * Copyright (c) 2022 axelccccc
 * 
 */

#include <string.h>

int memcmp(void* src, void* dest, size_t count) {

    unsigned char* src8 = (unsigned char*)src;
    unsigned char* dest8 = (unsigned char*)dest;

    for(size_t i = 0; i < count; i++) {
        if(src8[i] != dest8[i]) {
            return src8[i] < dest8[i] ? -1 : 1;
        }
    }

    return 0;
    
}