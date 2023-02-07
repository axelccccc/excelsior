#include <stdio.h>
#include <stdlib.h>

#if defined(__i386__)

// Arch specific (i386)
// [include in corresponding feature kernel headers !!]

#include <arch/i386/gdt.h>
#include <arch/i386/idt.h>
#include <arch/i386/pic.h>
#include <arch/i386/mmu.h>

#endif

#include <kernel/term.h>
#include <kernel/mem.h>

#if defined(__linux__)
#error "Wrong OS target"
#endif

#if !defined(__i386__)
#error "This kernel must be compiled for an ix86 target (-m elf_i386)"
#endif

extern int _init(void);
extern int _fini(void);
void main(void);

__attribute__((noreturn))
void _start(void) {
    _init();
    main();
    __asm__ volatile("hlt");
    __builtin_unreachable();
}

void _end(void) {
    _fini();
}

__attribute__((constructor)) void test(void) {
    term_init();
    term_clear();
    puts("This prints before main()");
}

/**
 * @BUG: When compiled with `-g`, this function is never called ?
 * @FIX: Stripping debug symbols from binary and debugging with
 *       separate symbol file fixes the problem ? weird.
 */
__attribute__((destructor)) void test2(void) {
    puts("This prints after main()");
    while(true) {}
}

// Global Descriptor Table
static uint64_t gdt[6];

// Interrupt Descriptor Table
// (256 * 8-byte structures)

static uint64_t idt[256];
extern void* isr_stub_table[];

void main(void) {

    uint8_t* gdt_bootloader = (uint8_t*)GDT_START;
    gdt_entry gdt_tmp;
    
    puts("Welcome to my OS!");

    // Disabling interrupts, just in case
    printf("Disabling Interrupts...");
    disable_interrupts();
    printf(" Done\n");

    printf("Relocating GDT...");
    set_gdt_entry(*get_gdt_entry(&gdt_bootloader[0], &gdt_tmp), (uint8_t*)&gdt[0]);
    set_gdt_entry(*get_gdt_entry(&gdt_bootloader[1*8], &gdt_tmp), (uint8_t*)&gdt[1]);
    set_gdt_entry(*get_gdt_entry(&gdt_bootloader[2*8], &gdt_tmp), (uint8_t*)&gdt[2]);
    set_gdt_entry(*get_gdt_entry(&gdt_bootloader[3*8], &gdt_tmp), (uint8_t*)&gdt[3]);
    set_gdt_entry(*get_gdt_entry(&gdt_bootloader[4*8], &gdt_tmp), (uint8_t*)&gdt[4]);
    set_gdt_entry(*get_gdt_entry(&gdt_bootloader[5*8], &gdt_tmp), (uint8_t*)&gdt[5]);
    printf(" Done\n");

    printf("Loading GDT...");
    load_gdt((uint32_t)&gdt, sizeof(gdt) - 1);
    printf(" Done\n");

    // Relocate PIC interrupts from 0x0 to 0x20
    // (0x0 -> 0x1F (the 32 first) are reserved for CPU exceptions)
    printf("Relocating PIC interrupts (0x00 -> 0x20)...");
    pic_remap(0x20, 0x28);
    printf(" Done\n");

    // Populate & load IDT
    printf("Loading IDT...");

    // 32 CPU Exceptions + 16(-1) PIC IRQs (-IRQ2: slave)
    for(int i = 0; i < 48; i++) {
        idt_entry entry = {
            .selector = 0x08,
            .offset = (uint32_t)isr_stub_table[i],
            .gate_type = G_TYPE_INT_32,
            .flags = G_PRESENT | G_PRIV_KERNEL
        };
        set_idt_entry(entry, (uint8_t*)&idt[i]);
    }

    load_idt((uint32_t)idt, sizeof(idt) - 1);

    printf(" Done\n");

    printf("Masking PIC IRQs...");
    for(int i = 0; i < 16; i++) {
        irq_set_mask(i);
    }
    printf(" Done\n");

    printf("Enabling supported PIC IRQs...");
    irq_clear_mask(1);
    printf(" Done\n");

    // Enable interrupts
    printf("Enabling Interrupts...");
    enable_interrupts();
    printf(" Done\n");

    // printf("PIC IRR: ");
    // printf("%04x\n", pic_get_irr());
    // printf("PIC ISR: ");
    // printf("%04x\n", pic_get_isr());

    while(true) {}

    _end();
    
}