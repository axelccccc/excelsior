#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "term.h"

// #if defined(__linux__)
// #error "Wrong OS target"
// #endif

#if !defined(__i386__)
#error "This kernel must be compiled for an ix86 target (-m elf_i386)"
#endif

extern int _init(void);
void main(void);

void _start(void) {
    _init();
    main();
}

__attribute__((constructor)) void test(void) {
    term_init();
    term_clear();
    term_print("This prints before main()\n");
}

void main(void) {

    term_print("Welcome to my OS!\n");
    term_print("Test another line\n");
    term_print("And another");
    while(true) {}
    
}