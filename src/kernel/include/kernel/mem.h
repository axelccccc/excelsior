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

#if defined(__i386__)
#include <arch/i386/mem.h>
#endif

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
 * @brief Memory map structure
 * (simple array of memory blocks)
 */
typedef struct {
    mblk* arr;
    size_t size;
} mem_map_t;

/**
 * @brief Global memory map (in mem.c)
 */
extern mem_map_t mem_map;

/**
 * @brief Get initial mapped memory entries
 * @param dst Destination array to store the entries
 * @return int Number of written entries
 */
int get_init_mem_map(mem_map_t* dst);

/**
 * @brief Set the global memory map
 * @param map Memory map to be used
 */
void set_mem_map(mem_map_t* map);

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