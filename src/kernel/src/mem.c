#include "arch/i386/mem.h"
#include <stdlib.h>
#include <stdio.h>

#include <kernel/mem.h>

#if defined(__i386__)
#include <arch/i386/e820.h>
#endif

/**
 * @brief Value defined in the linker script
 * located at the very end of the kernel.
 * It is not defined, what we use is its address
 * to locate the end of the kernel in memory.
 */
extern uint32_t kernel_end;

/**
 * @brief Global memory map's memory block array
 */
static mblk mem_map_arr[MEM_MAX_ENTRIES];

/**
 * @brief Global memory map
 */
mem_map_t mem_map = {
    .arr = mem_map_arr,
    .size = MEM_MAX_ENTRIES
};

/**
 * @brief Local function to map the kernel
 * physical memory in the memory map
 * @param map map to alter
 * @return int `0` on success, `-1` on failure
 */
int map_kernel(mem_map_t* map) {

    // Get kernel size
    uint32_t kernel_size = (uint32_t)&kernel_end - (uint32_t)MEM_KERNEL_START;

    // Get kernel block
    mblk mblk_kernel = {
        0,
        1,
        (uint32_t)MEM_KERNEL_START,
        kernel_size
    };

    // Find entry colliding with the kernel
    for(uint32_t i = 0; i < map->size; i++) {
        if(map->arr[i].addr <= MEM_KERNEL_START &&
           map->arr[i].addr + map->arr[i].size >= (uint32_t)&kernel_end) {

            // If the entry is free, split it
            if(!map->arr[i].used) {

                // Create new entry for the kernel
                mblk mblk_kernel = {
                    0,
                    1,
                    (uint32_t)MEM_KERNEL_START,
                    kernel_size
                };

                // Create new entry for the remaining space
                mblk mblk_free = {
                    0,
                    0,
                    (uint32_t)MEM_KERNEL_START + kernel_size,
                    map->arr[i].size - kernel_size
                };

                // Shift all entries after the current one
                for(uint32_t j = map->size - 1; j > i; j--) {
                    map->arr[j] = map->arr[j-1];
                }

                // Insert new entries
                map->arr[i] = mblk_kernel;
                map->arr[i+1] = mblk_free;

                return 0;

            }
            
        }
    }

    return -1;

}

int get_init_mem_map(mem_map_t* dst) {

// If arch i386, use BIOS INT18 E820
// data structures
#if defined(__i386__)

    // currently accessed mblk
    uint32_t cur_blk = 0;

    // Get data populated by the BIOS interrupt
    uint32_t mem_map_size = *((uint32_t*)E820_SIZE);
    e820_entry* ext_mem_map = (e820_entry*)E820_START;

    // Sort entries if necessary
    qsort(
        ext_mem_map,
        mem_map_size,
        sizeof(e820_entry),
        e820_entry_cmp
    );

    for(uint32_t i = 0; i < mem_map_size; i++) {

        // If mblk cannot add more entries, return
        if(cur_blk >= dst->size - 1) break;
        
        // Entry corresponding to E820 counterpart
        mblk entry = {
            0,
            ext_mem_map[i].type != 1,
            ext_mem_map[i].base,
            ext_mem_map[i].size
        };

        // Entry for if there is space between
        // current entry and next one
        mblk free_mem = {
            0,
            0,
            0,
            0
        };

        // Free space between two mem entries
        uint32_t free_space = 0;

        if(i < mem_map_size - 1) {
            free_space = ext_mem_map[i+1].base - ext_mem_map[i].size;
        } else {
            // We put all free space above last entry
            // until 0xffffffff into the last memory block
            free_space = UINT32_MAX - ext_mem_map[i].size;   
        }

        // If free space, allocate free block
        if(free_space > ext_mem_map[i].base) {
            free_mem.eom = 0;
            free_mem.used = 0;
            free_mem.addr = ext_mem_map[i].base + ext_mem_map[i].size;
            free_mem.size = free_space - ext_mem_map[i].base;
        }

        dst->arr[cur_blk++] = entry;
        if(free_mem.size > 0) {
            dst->arr[cur_blk++] = free_mem;
        }
        
    }

    if(map_kernel(dst) < 0)
        return -1;

    return ++cur_blk;
    
#endif

}

void set_mem_map(mem_map_t* map) {
    mem_map.arr = map->arr;
    mem_map.size = map->size;
}

void merge_free_blks(mem_map_t* map) {

    int adj = 0; // # of adjacent free blocks
    size_t i;
    size_t j;

    for(i = 0; i < map->size - 1; i++) {

        // If adjacent free blocks,
        // add lengths to first one
        while(!map->arr[i].used &&
              !map->arr[i+1].used &&
              i+adj+1 < map->size) {

            map->arr[i].size += map->arr[i+adj+1].size;
            ++adj;
            
        }

        // If adjacent blocks to merge...
        if(adj) {
            // ...slide `adj` blocks to the left
            for(j = i+1; j < map->size-adj; j++) {
                map[j] = map[j+adj];
            }
            // clear remaining blocks
            for(j = map->size-adj-1; j < map->size; j++) {
                map->arr[j].eom = 0;
                map->arr[j].used = 0;
                map->arr[j].addr = 0;
                map->arr[j].size = 0;
            }
        }

    }
    
}

void mem_print_blk(mblk blk) {
    printf("EOM:    %d\n", blk.eom);
    printf("Used:   %d\n", blk.used);
    printf("Addr:   0x%08x\n", blk.addr);
    printf("Length: 0x%08x\n", blk.size);
}