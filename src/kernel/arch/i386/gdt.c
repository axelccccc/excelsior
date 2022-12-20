#include <stdio.h>

#include "gdt.h"

struct gdt_entry* get_gdt_entry(uint8_t* src, struct gdt_entry* dst) {

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

void set_gdt_entry(struct gdt_entry src, uint8_t* dst) {

    dst[0] = src.limit & 0xFF;
    dst[1] = (src.limit >> 8) & 0xFF;
    dst[6] = (src.limit >> 16) & 0x0F;
 
    dst[2] = src.base & 0xFF;
    dst[3] = (src.base >> 8) & 0xFF;
    dst[4] = (src.base >> 16) & 0xFF;
    dst[7] = (src.base >> 24) & 0xFF;
 
    dst[5] = ((src.flags & 0x0F) << 4) | (src.flags & 0xF00);
    dst[6] |= ((src.flags & 0xF0) << 4);
    
}

void load_gdt() {

    __asm__ volatile( // volatile because has no output, not to be optimized away
        "jmp $0x08,$gdt_did_reset\n\t" // 0x08: kernel code segment
        "gdt_did_reset:\n\t"
        "mov $0x10, %ax\n\t"         // 0x10: kernel data segment
        "mov %ax, %ds\n\t"
        "mov %ax, %ss\n\t"
        "mov %ax, %es\n\t"
        "mov %ax, %fs\n\t"
        "mov %ax, %gs\n\t"
    );
    
}

void print_gdt_entry_info(struct gdt_entry* entry) {

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