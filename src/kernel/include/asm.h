#pragma once

/**
 * @brief Reads a byte from the specified port
 * "=a" (result) means : put AL register in variable RESULT when finished
 * "d" (port) means : load EDX with port
 * @param port Port to read from
 * @return unsigned char Byte read from port
 */
unsigned char port_byte_in(unsigned short port);

/**
 * @brief Writes a byte to the specified port
 * @param port Port to write to
 * @param data Byte of data to write
 */
void port_byte_out(unsigned short port, unsigned char data);

/**
 * @brief Reads a word from the specified port
 * @param port Port to read from
 * @return unsigned char Word read from port
 */
unsigned short port_word_in(unsigned short port);

/**
 * @brief Writes a word to the specified port
 * @param port Port to write to
 * @param data Word of data to write
 */
void port_word_out(unsigned short port, unsigned short data);