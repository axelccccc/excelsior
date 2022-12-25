#ifndef _KERNEL_ASM_H
#define _KERNEL_ASM_H 1

#include <stdint.h>

/**
 * @brief Reads a byte from the specified port
 * "=a" (result) means : put AL register in variable RESULT when finished
 * "d" (port) means : load EDX with port
 * @param port Port to read from
 * @return unsigned char Byte read from port
 */
static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile(
        "in %%dx, %%al"
        : "=a" (result)
        : "d" (port)
    );
    return result;
}

/**
 * @brief Writes a byte to the specified port
 * @param port Port to write to
 * @param data Byte of data to write
 */
static inline void outb(uint16_t port, uint8_t data) {
    __asm__ volatile(
        "out %%al, %%dx"
        :
        : "a" (data), "d" (port)
    );
}

/**
 * @brief Reads a word from the specified port
 * @param port Port to read from
 * @return unsigned char Word read from port
 */
static inline uint16_t inw(uint16_t port) {
    unsigned short result;
    __asm__ volatile(
        "in %%dx, %%ax"
        : "=a" (result)
        : "d" (port)
    );
    return result;
}

/**
 * @brief Writes a word to the specified port
 * @param port Port to write to
 * @param data Word of data to write
 */
static inline void outw(uint16_t port, uint16_t data) {
    __asm__ volatile(
        "out %%ax, %%dx"
        :
        : "a" (data), "d" (port)
    );
}

/**
 * @brief Reads a double word from the specified port
 * @param port Port to read from
 * @return unsigned char Double word read from port
 */
static inline uint32_t inl(uint16_t port) {
    uint32_t result;
    __asm__ volatile(
        "in %%dx, %%eax"
        : "=a" (result)
        : "d" (port)
    );
    return result;
}

/**
 * @brief Writes a double word to the specified port
 * @param port Port to write to
 * @param data Double word of data to write
 */
static inline void outl(uint16_t port, uint32_t data) {
    __asm__ volatile(
        "out %%eax, %%dx"
        :
        : "a" (data), "d" (port)
    );
}

/**
 * @brief Do an useless I/O operation
 * to wait a some time (generally 1-4us)
 */
static inline void io_wait(void) {
    outb(0x80, 0);
}

/**
 * @brief Read a byte from another segment
 * than the C data segment (segment:offset)
 * @param sel Segment to read from
 * @param off Offset in segment to read from
 * @return uint8_t Byte read from segment:offset
 */
static inline uint8_t farpeekb(uint16_t sel, void *off) {
    uint8_t ret;
    __asm__ volatile(
        "push %%fs\n\t"
        "mov %1, %%fs\n\t"
        "mov %%fs:(%2), %0\n\t"
        "pop %%fs"
        : "=r" (ret)
        : "g" (sel), "r" (off)
    );
    return ret;
}

/**
 * @brief Read a word from another segment
 * than the C data segment (segment:offset)
 * @param sel Segment to read from
 * @param off Offset in segment to read from
 * @return uint16_t Word read from segment:offset
 */
static inline uint16_t farpeekw(uint16_t sel, void *off) {
    uint16_t ret;
    __asm__ volatile(
        "push %%fs\n\t"
        "mov %1, %%fs\n\t"
        "mov %%fs:(%2), %0\n\t"
        "pop %%fs"
        : "=r" (ret)
        : "g" (sel), "r" (off)
    );
    return ret;
}

/**
 * @brief Read a double word from another segment
 * than the C data segment (segment:offset)
 * @param sel Segment to read from
 * @param off Offset in segment to read from
 * @return uint32_t Double word read from segment:offset
 */
static inline uint32_t farpeekl(uint16_t sel, void *off) {
    uint32_t ret;
    __asm__ volatile(
        "push %%fs\n\t"
        "mov %1, %%fs\n\t"
        "mov %%fs:(%2), %0\n\t"
        "pop %%fs"
        : "=r" (ret)
        : "g" (sel), "r" (off)
    );
    return ret;
}

#endif