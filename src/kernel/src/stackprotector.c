#include <stdint-gcc.h>
#include <stdint.h>
#include <stdlib.h>


// TODO: Randomly generated value
#if UINT32_MAX == UINTPTR_MAX
#define STACK_CHK_GUARD 0xb3f84ea1
#else
#define STACK_CHK_GUARD 0x688e2fbc52e1ff69
#endif

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

__attribute__((noreturn))
void __stack_chk_fail(void) {
#if __STDC_HOSTED__
    abort();
#elif __is_kernel
    abort();
#endif
}