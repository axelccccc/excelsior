#include <stdio.h>
#include <stdlib.h>

__attribute__((noreturn))
void abort(void) {
#ifdef __is_libk
    // TODO: Add proper kernel panic.
    puts("kernel: panic: abort()");
#else
    // TODO: Abnormally terminate the process as if by SIGABRT.
    puts("abort()");
#endif
    while(1) {}
    __builtin_unreachable();
}