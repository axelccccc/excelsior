/**
 * @file mem.h
 * @author axelccccc (github.com/axelccccc)
 * @brief Kernel memory management
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

// Start of extended memory (> 1mb)
#define MEM_EXT_START       (1 << 20)

// Max entries the mblk array can support
#define MEM_MAX_ENTRIES     256

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
 * @brief Global memory map
 */
extern mblk* mem_map;

/**
 * @brief Global memory map size
 */
extern size_t mem_map_size;

/**
 * @brief Get initial mapped memory entries
 * @param dst Destination array to store the entries
 * @param size Size of the destination array
 * @return int Number of written entries
 */
int get_init_mem_map(mblk* dst, size_t size);

/**
 * @brief Set the global memory map
 * @param map Memory map to be used
 */
void set_mem_map(mblk* map, size_t size);

/**
 * @brief Merge all adjacent free memory blocks
 * from provided memory map
 * (used in free())
 * @param map Memory map to alter
 * @param size Number of entries in the memory map
 */
void merge_free_blks(mblk* map, size_t size);

/**
 * @brief Print information on a memory block
 * @param blk Memory block to get information on
 */
void mem_print_blk(mblk blk);

#endif