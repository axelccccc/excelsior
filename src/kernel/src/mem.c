#include <stdlib.h>
#include <stdio.h>

#include <kernel/mem.h>

#if defined(__i386__)
#include <e820.h>
#endif

/**
 * @brief Global memory map
 */
mblk* mem_map;

/**
 * @brief Global memory map size
 */
size_t mem_map_size;

int get_init_mem_map(mblk* dst, size_t size) {

// If arch i386, use BIOS INT18 E820
// data structures
#if defined(__i386__)

    // currently accessed mblk
    uint32_t cur_blk = 0;

    // Get data populated by the BIOS interrupt
    uint32_t mem_map_size = *((uint32_t*)E820_SIZE);
    e820_entry* ext_mem_map = (e820_entry*) E820_START;

    // Sort entries if necessary
    qsort(
        ext_mem_map,
        mem_map_size,
        sizeof(e820_entry),
        e820_entry_cmp
    );

    for(uint32_t i = 0; i < mem_map_size; i++) {

        // If mblk cannot add more entries, return
        if(cur_blk >= size - 1) break;
        
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

        dst[cur_blk] = entry;
        cur_blk++;
        if(free_mem.size > 0) {
            dst[cur_blk] = free_mem;
            cur_blk++;
        }
        
    }

    return cur_blk;
    
#endif

}

void set_mem_map(mblk* map, size_t size) {
    mem_map = map;
    mem_map_size = size;
}

void merge_free_blks(mblk* map, size_t size) {

    int adj = 0; // # of adjacent free blocks
    size_t i;
    size_t j;

    for(i = 0; i < size - 1; i++) {

        // If adjacent free blocks,
        // add lengths to first one
        while(!map[i].used &&
              !map[i+1].used &&
              i+adj+1 < size) {

            map[i].size += map[i+adj+1].size;
            ++adj;
            
        }

        // If adjacent blocks to merge...
        if(adj) {
            // ...slide `adj` blocks to the left
            for(j = i+1; j < size-adj; j++) {
                map[j] = map[j+adj];
            }
            // clear remaining blocks
            for(j = size-adj-1; j < size; j++) {
                map[j].eom = 0;
                map[j].used = 0;
                map[j].addr = 0;
                map[j].size = 0;
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