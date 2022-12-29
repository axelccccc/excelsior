/**
 * @file qsort.c
 * @author axelccccc (github.com/axelccccc)
 * @brief Implementation of the qsort function
 * @version 0.1
 * @date 2022-12-28
 * 
 * Copyright (c) 2022 axelccccc
 * 
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void qsort(
    void* base,
    size_t num,
    size_t size,
    int (*cmp)(const void*, const void*)) {

    if(num <= 1 || !size) return;

    uint8_t* thresh = (uint8_t*)base;
    uint8_t* lh     = (uint8_t*)base + size;
    uint8_t* rh     = (uint8_t*)base + size * (num - 1);
    // Temporary elmt for swapping
    // (size allocated to avoid overlap)
    uint8_t tmp[size];

    int arr1_len = 0;
    int arr2_len = 0;

    while(lh != rh) {

        // Move the rh to the left until it either
        // coincides with the lh, or points to a value
        // smaller than the pivot
        while(lh != rh) {
            int x = cmp(rh, thresh);
            if(x >= 0) rh -= size;
            else break;
        }

        // Move lh to the right until it coincides
        // with rh or points to an element larger or
        // equal to the pivot
        while(lh != rh) {
            int x = cmp(lh, thresh);
            if(x < 0) lh += size;
            else break;
        }

        // If lh and rh have not yet reached the same
        // position, swap their values
        if(lh != rh) {
            memcpy(tmp, rh, size); // rh -> tmp
            memcpy(rh, lh, size);  // lh -> rh
            memcpy(lh, tmp, size); // tmp(rh) -> lh
        }

    } // while(lh != rh)

    // Unless the pivot was the smallest elmt
    // in the array, lh(rh) is smaller, swap
    if(cmp(base, lh) > 0) {
        memcpy(tmp, lh, size);      // lh -> tmp
        memcpy(lh, base, size);     // base -> lh
        memcpy(base, tmp, size);    // tmp(lh) -> base
    }

    // Repeat the algorithm on each side of lh(rh)
    // (divide-and-conquer)
    arr1_len = (lh - (uint8_t*)base) / size;
    arr2_len = (num - (lh - (uint8_t*)base) / size);
    qsort(base, arr1_len, size, cmp);
    qsort(lh, arr2_len, size, cmp);

}