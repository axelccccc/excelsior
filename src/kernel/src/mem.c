#include "mem.h"

void mem_cpy(char* src, char* dest, size_t count) {
    for(size_t i = 0; i < count; i++) {
        dest[i] = src[i];
    }
}

void mem_set(char* dest, char val, size_t count) {
    for(size_t i = 0; i < count; i++) {
        dest[i] = val;
    }
}