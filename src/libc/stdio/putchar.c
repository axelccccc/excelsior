#include <stdio.h>

#ifdef __is_libk
#include <kernel/term.h>
#endif

void putchar(char c) {

#ifdef __is_libk

    term_putchar(c);

#else



#endif

}