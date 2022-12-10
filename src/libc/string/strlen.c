/**
 * @file strlen.c
 * @author axelccccc (github.com/axelccccc)
 * @brief Get the length of a string
 * @version 0.1
 * @date 2022-12-10
 * 
 * Copyright (c) 2022 axelccccc
 * 
 */

#include <string.h>

size_t strlen(const char* str) {
    
    size_t len = 0;
    
    while(str[len])
        len++;

    return len;

}