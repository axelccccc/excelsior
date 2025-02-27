#include <stdio.h>

#include "gdt.h"

gdt_entry* get_gdt_entry(uint8_t* src, gdt_entry* dst) {

    dst->limit = (src[0] | (src[1] << 8));
    dst->limit |= ((src[6] & 0x0F) << 16);

    dst->base  = (src[2] | (src[3] << 8));
    dst->base  |= (src[4] << 16);
    dst->base  |= (src[7] << 24);

    dst->flags = (src[5] & 0xF0) >> 4;          // 1st flags
    dst->flags |= (src[6] & 0xF0);         // 2nd flags
    dst->flags |= (src[5] & 0x0F) << 8;    // type flags
    
    return dst;
    
}

void set_gdt_entry(gdt_entry* src, uint8_t* dst) {

    dst[0] = src->limit & 0xFF;
    dst[1] = (src->limit >> 8) & 0xFF;
    dst[6] = (src->limit >> 16) & 0x0F;
 
    dst[2] = src->base & 0xFF;
    dst[3] = (src->base >> 8) & 0xFF;
    dst[4] = (src->base >> 16) & 0xFF;
    dst[7] = (src->base >> 24) & 0xFF;
 
    dst[5] = ((src->flags & 0x0F) << 4) | ((src->flags & 0xF00) >> 8);
    dst[6] |= (src->flags & 0xF0);
    
}

int get_gdt_array(uint8_t* dst, size_t size) {

    int written = 0;
    uint8_t* gdt_bootloader = (uint8_t*)GDT_START;
    gdt_entry gdt_tmp;

    for(size_t i = 0; i < size; i++) {
        set_gdt_entry(
            get_gdt_entry(
                &gdt_bootloader[i*8],
                &gdt_tmp),
            &dst[i*8]
        );
        ++written;
    }

    return written;

}

/**
 * BUG: LGDT does not load descriptor properly for an address over 2 bytes ??
 * FIX: It was just set_gdt_entry() that was wrong...
 */
void load_gdt(uint32_t base, uint16_t limit) {

    struct {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed)) _gdt = { limit, base };

    __asm__ volatile( // volatile because has no output, not to be optimized away
        "lgdt (%0)\n\t"
        "mov $0x10, %%ax\n\t"         // 0x10: kernel data segment
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%ss\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "jmp $0x08, $gdt_did_reset\n\t" // 0x08: kernel code segment
        "gdt_did_reset:\n\t"
        :   // double colon (empty constraints) enable extended asm,
            // for which registers are prefixed with %% instead of %
            // (escaping %)
        : "r" (&_gdt) // input
    );
    
}

void print_gdt_entry_info(gdt_entry* entry) {

    printf("Limit      : %08x\n", entry->limit);
    printf("Base       : %08x\n", entry->base);
    printf("Flags A    : %01x\n", (entry->flags & 0xF));
    printf("    Present     : %01x\n", (entry->flags & GDT_FLAG_PRESENT) >> 3);
    printf("    Privilege   : %01x\n", (entry->flags & GDT_FLAG_PRIVILEGE) >> 1);
    printf("    Desc type   : %01x\n", (entry->flags & GDT_FLAG_DESC_TYPE));

    printf("Flags B    : %01x\n", ((entry->flags >> 4) & 0xF));
    printf("    Granularity : %01x\n", (entry->flags & GDT_FLAG_GRANULARITY) >> 7);
    printf("    Default 32  : %01x\n", (entry->flags & GDT_FLAG_DEFAULT_32) >> 6);
    printf("    Seg 64      : %01x\n", (entry->flags & GDT_FLAG_SEG_64) >> 5);
    printf("    AVL         : %01x\n", (entry->flags & GDT_FLAG_AVL) >> 4);

    printf("Type flags : %01x\n", ((entry->flags >> 8) & 0xF));
    printf("    Code        : %01x\n", (entry->flags & GDT_TYPE_CODE) >> 11);
    printf("    Conforming  : %01x\n", (entry->flags & GDT_TYPE_CONFORMING) >> 10);
    printf("    RW          : %01x\n", (entry->flags & GDT_TYPE_RW) >> 9);
    printf("    Accessed    : %01x\n", (entry->flags & GDT_TYPE_ACCESSED) >> 8);
    
}