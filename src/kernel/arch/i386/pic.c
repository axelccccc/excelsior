#include "asm.h"
#include "pic.h"

// OCW3 PIC Read Register Commands
// (Operating Command Word)

#define PIC_REG_IRR             0x0A        // Interrupt request register
#define PIC_REG_ISR             0x0B        // In-service register

void pic_send_eoi(uint8_t irq) {
    if(irq >= 8)
        outb(PIC2_CMD, PIC_EOI);
    outb(PIC1_CMD, PIC_EOI);
}

void pic_remap(uint8_t offset1, uint8_t offset2) {
    
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);                // save current masks
    a2 = inb(PIC2_DATA);

    outb(PIC1_CMD, PIC_ICW1_INIT | PIC_ICW1_ICW4); // start initialization sequence (cascade mode)
    io_wait();                          // give time for PIC to process commands for older machines
    outb(PIC2_CMD, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, offset1);           // ICW2: Master PIC vector offset
    io_wait();
    outb(PIC2_DATA, offset2);           // ICW2: Slave PIC vector offset
    io_wait();

    outb(PIC1_DATA, 4);                 // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    outb(PIC2_DATA, 2);                 // ICW3: tell Slave PIC its cascade identity (0000 0010)
    io_wait();

    outb(PIC1_DATA, PIC_ICW4_8086);     // ICW4: 8086 mode
    io_wait();
    outb(PIC2_DATA, PIC_ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);                // restore saved masks.
    io_wait();
    outb(PIC2_DATA, a2);

}

void irq_set_mask(uint8_t irq_line) {
    uint16_t port;
    uint8_t value;

    if(irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    value = inb(port) | (1 << irq_line);
    outb(port, value);
}

void irq_clear_mask(uint8_t irq_line) {
    uint16_t port;
    uint8_t value;

    if(irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    value = inb(port) & ~(1 << irq_line);
    outb(port, value);
}

/**
 * @brief Get the value of a PIC register
 * 
 * @param reg OCW3 command word to send
 * @return uint16_t Value of the register
 */
static uint16_t _pic_get_reg(int reg) {

    outb(PIC1_CMD, reg);
    outb(PIC2_CMD, reg);

    return (inb(PIC2_CMD) << 8) | inb(PIC1_CMD);
    
}

/**
 * @brief Get the combined value of the cascaded PICs'
 * interrupt request register
 * @return uint16_t Value of the IRR
 */
uint16_t pic_get_irr(void) {
    return _pic_get_reg(PIC_REG_IRR);
}

/**
 * @brief Get the combined value of the cascaded PICs'
 * in-service register
 * @return uint16_t Value of the ISR
 */
uint16_t pic_get_isr(void) {
    return _pic_get_reg(PIC_REG_ISR);
}