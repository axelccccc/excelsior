#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdio.h>
#include <kernel/term.h>

#include <gdt.h>

#if defined(__linux__)
#error "Wrong OS target"
#endif

#if !defined(__i386__)
#error "This kernel must be compiled for an ix86 target (-m elf_i386)"
#endif

extern int _init(void);
extern int _fini(void);
void main(void);

void _start(void) {
    _init();
    main();
}

void _end(void) {
    _fini();
}

/* __attribute__((constructor)) void test(void) {
    term_init();
    term_clear();
    puts("This prints before main()");
} */

/**
 * @BUG: When compiled with `-g`, this function is never called ?
 */
/* __attribute__((destructor)) void test2(void) {
    puts("This prints after main()");
    while(true) {}
} */

void main(void) {

    uint64_t* gdt_start = (uint64_t*)0x7c15;

    struct gdt_entry k_code;
    struct gdt_entry k_data;
    struct gdt_entry u_code;
    struct gdt_entry u_data;
    struct gdt_entry tss;

    term_init();
    term_clear();

    puts("Welcome to my OS!");

    puts("Retrieving GDT entries...");

    get_gdt_entry((uint8_t*)(gdt_start+1), &k_code);
    get_gdt_entry((uint8_t*)(gdt_start+2), &k_data);
    get_gdt_entry((uint8_t*)(gdt_start+3), &u_code);
    get_gdt_entry((uint8_t*)(gdt_start+4), &u_data);
    get_gdt_entry((uint8_t*)(gdt_start+5), &tss);

    puts("Kernel Code");
    print_gdt_entry_info(&k_code);
    puts("Kernel Data");
    print_gdt_entry_info(&k_data);
    puts("User Code");
    print_gdt_entry_info(&u_code);
    puts("User Data");
    print_gdt_entry_info(&u_data);
    puts("TSS");
    print_gdt_entry_info(&tss);

    while(true) {}

    // _end();
    
}