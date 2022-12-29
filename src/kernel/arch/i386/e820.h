/**
 * @file e820.h
 * @author axelccccc (github.com/axelccccc)
 * @brief Intel x86's BIOS INT18 E820 memory map
 * @version 0.1
 * @date 2022-12-28
 * 
 * Copyright (c) 2022 axelccccc
 * 
 */

#ifndef _KERNEL_E820_H
#define _KERNEL_E820_H 1

#include <stdint.h>

// Address of INT 18 E820's array size
#define E820_SIZE       0x7e00

// Address of INT 18 E820's array start
#define E820_START      0x7e04

typedef struct {
    uint64_t base;
    uint64_t size;
    uint32_t type;
    uint32_t acpi3;
} e820_entry;

/**
 * @brief Comparison function of e820 entries for qsort
 * @param a Entry 1
 * @param b Entry 2
 * @return int Comparison result
 */
int e820_entry_cmp(const void* a, const void* b);

/**
 * @brief Print e820 entry information
 * @param entry Entry to be printed
 */
void e820_print_entry(e820_entry entry);

#endif