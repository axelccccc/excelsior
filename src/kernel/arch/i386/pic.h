#ifndef _KERNEL_PIC_H
#define _KERNEL_PIC_H 1

#include <stdint.h>

// PIC Lines

#define PIC1                    0x20        // IO base address for master PIC
#define PIC2                    0xA0        // IO base address for slave PIC
#define PIC1_CMD                PIC1        // Master PIC command port
#define PIC1_DATA               (PIC1+1)    // Master PIC data port
#define PIC2_CMD                PIC2        // Slave PIC command port
#define PIC2_DATA               (PIC2+1)    // Slave PIC data port

// PIC Commands

#define PIC_EOI                 0x20        // End-of-interrupt command code

#define PIC_ICW1_ICW4           0x01        // ICW4 (not) needed
#define PIC_ICW1_SINGLE         0x02        // Single (cascade) mode
#define PIC_ICW1_INTERVAL4      0x04        // Call address interval 4 (8)
#define PIC_ICW1_LEVEL          0x08        // Level triggered (edge) mode
#define PIC_ICW1_INIT           0x10        // Initialization

#define PIC_ICW4_8086           0x01        // 8086/88 (MCS-80/85) mode
#define PIC_ICW4_AUTO           0x02        // Auto (normal) EOI
#define PIC_ICW4_BUF_SLAVE      0x08        // Buffered mode/slave
#define PIC_ICW4_BUF_MASTER     0x0C        // Buffered mode/master
#define PIC_ICW4_SFNM           0x10        // Special fully nested (not)

/**
 * @brief Send end-of-interrupt command to PIC
 * @param irq IRQ number
 */
void pic_send_eoi(uint8_t irq);

/**
 * @brief Remap the PIC vector offsets
 * 
 * @param offset1 Offset for master PIC
 * @param offset2 Offset for slave PIC
 */
void pic_remap(uint8_t offset1, uint8_t offset2);

/**
 * @brief Set the mask for a specific IRQ line
 * 
 * @param irq_line IRQ line to set mask for
 */
void irq_set_mask(uint8_t irq_line);

/**
 * @brief Clear the mask for a specific IRQ line
 * 
 * @param irq_line IRQ line to clear mask for
 */
void irq_clear_mask(uint8_t irq_line);

uint16_t pic_get_irr(void);
uint16_t pic_get_isr(void);

#endif