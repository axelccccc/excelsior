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

int memcmp(const void* itm1, const void* itm2, size_t count) {

    unsigned char* itm1_8 = (unsigned char*)itm1;
    unsigned char* itm2_8 = (unsigned char*)itm2;

    for(size_t i = 0; i < count; i++) {
        if(itm1_8[i] != itm2_8[i]) {
            return itm1_8[i] < itm2_8[i] ? -1 : 1;
        }
    }

    return 0;
    
}