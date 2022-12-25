#include <stdio.h>

#include <asm.h>
#include <pic.h>

__attribute__((noreturn))
void exception_handler(void) {
    // __asm__ volatile(
    //     "\n\t"
    //     "cli\n\t"
    //     "hlt"
    // );
    puts("CPU: Exception");
    for(;;);
    __builtin_unreachable();
}

void irq_handler(void) {
    puts("PIC: IRQ");
    printf("PIC IRR: ");
    printf("%04x\n", pic_get_irr());
    printf("PIC ISR: ");
    printf("%04x\n", pic_get_isr());
    pic_send_eoi(pic_get_isr());
}

void kbd_handler(void) {

    uint16_t c = inb(0x60);
    putchar(c);
    pic_send_eoi(1);

}