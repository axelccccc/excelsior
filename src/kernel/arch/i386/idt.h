#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H 1

#include <stdint.h>

// Gate Types

#define G_TYPE_TASK     0x5     // Task gate
#define G_TYPE_INT_16   0x6     // 16-bit interrupt gate
#define G_TYPE_TRAP_16  0x7     // 16-bit trap gate
#define G_TYPE_INT_32   0xe     // 32-bit interrupt gate
#define G_TYPE_TRAP_32  0xf     // 32-bit trap gate

// CPU Privilege Levels (DPL)

#define G_PRIV_KERNEL   0x0     // Kernel
#define G_PRIV_USER     0x6     // User

// Flags

#define G_PRESENT       0x8     // Present
#define G_ABSENT        0x0     // NOT present

typedef struct {

    uint32_t offset;    // ISR's address
    uint16_t selector;  // GDT segment selector to load into CS on interrupt
    uint8_t gate_type;  // Zero: 4, Gate Type: 4
    uint8_t flags;      // Zero: 4, Present: 1, Privileges: 3
    
} __attribute__((packed)) idt_entry;

/* typedef struct {

    uint16_t limit;
    uint32_t base;
    
} __attribute__((packed)) idt_descriptor; */

/**
 * @brief Get and decode an IDT entry from memory
 * @param src 
 * @param dst Pointer to the destination struct
 * @return struct idt_entry* 
 */
idt_entry* get_idt_entry(uint8_t* src, idt_entry* dst);

/**
 * @brief Encode and set an IDT Entry in memory
 * @param src Source struct
 * @param dst 
 */
void set_idt_entry(idt_entry src, uint8_t* dst);

/**
 * @brief Load the IDT into the processor
 * 
 * @param base Base address of the IDT
 * @param size Size of the IDT
 */
static inline void load_idt(uint32_t base, uint16_t size) {

    struct {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed)) _idt = { size, base };

    __asm__ volatile("lidt (%0)" : : "r"(&_idt));

}

/**
 * @brief Enable interrupts by setting the IF flag 
 * in the EFLAGS register of the CPU
 */
static inline void enable_interrupts() {
    __asm__ volatile("sti");
}

/**
 * @brief Disable interrupts by clearing the IF flag 
 * in the EFLAGS register of the CPU
 */
static inline void disable_interrupts() {
    __asm__ volatile("cli");
}

#endif