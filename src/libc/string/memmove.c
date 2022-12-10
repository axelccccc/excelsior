/**
 * @file memmove.c
 * @author axelccccc (github.com/axelccccc)
 * @brief Move memory blocks
 * @version 0.1
 * @date 2022-12-10
 * 
 * Copyright (c) 2022 axelccccc
 * 
 */

#include <string.h>

void* memmove(void* dest, void* src, size_t count) {

    unsigned char* dest8 = (unsigned char*)dest;
    unsigned char* src8 = (unsigned char*)src;

    // handles order of moving in case of overlapping
    if(dest8 < src8) { // from the start if dest starts before src
        for(size_t i = 0; i < count; i++) {
            dest8[i] = src8[i];
        }
    } else { // from the end if dest starts after src
        for(size_t i = count; i > 0; i--) {
            dest8[i - 1] = src8[i - 1];
        }
    }

    return dest;
    
}