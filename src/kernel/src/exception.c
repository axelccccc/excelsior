#include <stdio.h>

#include <asm.h>
#include <pic.h>
#include <ps2.h>

#include <kernel/kbd.h>

__attribute__((noreturn))
void exception_handler(void) {
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

/**
 * @brief Simple Keyboard interrupt handler
 * @TODO: Implement a proper keyboard driver
 * @TODO: Handle Alt & Ctrl
 */
void kbd_handler(void) {

    static uint8_t shift = 0;
    static uint8_t caps = 0;
    static uint8_t ctrl = 0;
    static uint8_t alt = 0;
    
    uint16_t c = inb(PS2_DATA_PORT);

    if (c == 0x2a || c == 0x36) {
        shift = 1;
    } else if (c == 0xaa || c == 0xb6) {
        shift = 0;
    } else if (c == 0x3a) {
        caps = !caps;
    } else if (c == 0x1d) {
        ctrl = 1;
    } else if (c == 0x9d) {
        ctrl = 0;
    } else if (c == 0x38) {
        alt = 1;
    } else if (c == 0xb8) {
        alt = 0;
    }

    if (c < 0x80) {
        if (shift) {
            if(layout_US_1[c + 128]) putchar(layout_US_1[c + 128]);
        } else {
            if(layout_US_1[c]) putchar(layout_US_1[c]);
        }
    }

    pic_send_eoi(1);

}