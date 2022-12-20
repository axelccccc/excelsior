#include <stdint.h>
#include <stdbool.h>

// Start of the Global Descriptor Table
// (temporary)

#define GDT_START               0x7c15

// The flags are defined in accordance to
// the way they are stored in gdt_entry.flags
//      NONE TYPE FLAGS B FLAGS A
// 0b   0000 0000 0000    0000

#define GDT_FLAG_PRESENT        0x08
#define GDT_FLAG_PRIVILEGE      0x06
#define GDT_FLAG_DESC_TYPE      0x01

#define GDT_FLAG_GRANULARITY    0x80
#define GDT_FLAG_DEFAULT_32     0x40
#define GDT_FLAG_SEG_64         0x20
#define GDT_FLAG_AVL            0x10

#define GDT_TYPE_CODE           0x800
#define GDT_TYPE_CONFORMING     0x400
#define GDT_TYPE_RW             0x200
#define GDT_TYPE_ACCESSED       0x100

struct gdt_entry {
        
    uint32_t base;
    uint32_t limit;
    uint16_t flags;

};

/**
 * @brief Get and decode a GDT entry from memory
 * @param src 
 * @param dst 
 * @return struct gdt_entry* 
 */
struct gdt_entry* get_gdt_entry(uint8_t* src, struct gdt_entry* dst);

/**
 * @brief Encode and set a GDT Entry in memory
 * @param src 
 * @param dst 
 */
void set_gdt_entry(struct gdt_entry src, uint8_t* dst);

/**
 * @brief Load Global Descriptor Table
 */
void load_gdt();

/**
 * @brief Print detailed information on a GDT entry
 * @param entry GDT entry to print
 */
void print_gdt_entry_info(struct gdt_entry* entry);