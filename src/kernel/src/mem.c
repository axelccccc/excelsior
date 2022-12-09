#include "mem.h"

void memcpy(char* src, char* dest, size_t count) {
    for(size_t i = 0; i < count; i++) {
        dest[i] = src[i];
    }
}

void memset(char* dest, char val, size_t count) {
    for(size_t i = 0; i < count; i++) {
        dest[i] = val;
    }
}