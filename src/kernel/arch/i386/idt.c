#include "idt.h"

idt_entry* get_idt_entry(uint8_t* src, idt_entry* dst) {
    
    dst->offset = src[0] | (src[3] << 16);
    dst->selector = src[1];
    dst->gate_type = src[2] >> 8;
    dst->flags = src[2] >> 12;

    return dst;

}

void set_idt_entry(idt_entry src, uint8_t* dst) {

    dst[0] = src.offset & 0xFF;
    dst[1] = (src.offset >> 8) & 0xFF;
    dst[2] = src.selector & 0xFF;
    dst[3] = (src.selector >> 8) & 0xFF;
    dst[4] = 0x00;
    dst[5] = ((src.flags & 0x0F) << 4) | (src.gate_type & 0x0F);
    dst[6] = (src.offset >> 16) & 0xFF;
    dst[7] = (src.offset >> 24) & 0xFF;
    
}