/**
 * @file mem.h
 * @author axelccccc (github.com/axelccccc)
 * @brief Kernel physical memory management
 * @version 0.1
 * @date 2022-12-27
 * 
 * Copyright (c) 2022 axelccccc
 * 
 */

#ifndef _KERNEL_MEM_H
#define _KERNEL_MEM_H 1

#include <stdint.h>
#include <stddef.h>

#if defined(__i386__)
#include <arch/i386/memdefs.h>
#endif

// Max entries the mblk array can support
#define MEM_MAX_ENTRIES     256

// Kernel start virtual address
#define KERNEL_START        0xC0000000

/**
 * @brief Value defined in the linker script
 * located at the very end of the kernel.
 * It is not defined, what we use is its address
 * to locate the end of the kernel in memory.
 */
extern uint32_t kernel_end;

/**
 * @brief Memory Block Header
 */
typedef struct {
    uint8_t  eom: 1;    // End of memory ?
    uint8_t  used: 1;   // Used ?
    uint32_t addr: 32;  // Address
    uint32_t size: 32;  // Size (bytes)
} __attribute__((packed)) mblk;

/**
 * @brief Memory map structure
 * (simple array of memory blocks)
 */
typedef struct {
    mblk* arr;
    size_t size;
} mem_map_t;

/**
 * @brief Global physical memory map (in mem.c)
 */
extern mem_map_t phys_mem;

/**
 * @brief Get initial mapped memory entries
 * @return int Number of written entries, `-1` on error
 */
int get_init_mem_map();

/**
 * @brief Insert memory block into the map
 * @param map map to alter
 * @return int `0` on success, `-1` on failure
 */
int insert_mblk(mem_map_t* map, mblk blk);

/**
 * @brief Merge all adjacent free memory blocks
 * from provided memory map
 * (used in free())
 * @param map Memory map to alter
 */
void merge_free_blks(mem_map_t* map);

/**
 * @brief Print information on a memory block
 * @param blk Memory block to get information on
 */
void mem_print_blk(mblk blk);

#endif