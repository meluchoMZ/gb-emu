/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#ifndef __BIN_UTILS_H__
#define __BIN_UTILS_H__

#include <stdint.h>

/**
 * Obtains the most significant byte from a 16 bit value
 */
uint8_t msb(uint16_t value);

/**
 * Obtains the least significant byte from a 16 bit value
 */
uint8_t lsb(uint16_t value);

/**
 * Converts two unsigned 8 bit integers to a 16 bit unsigned integer
 */
uint16_t toUint16(uint8_t msb, uint8_t lsb);

#endif // __BIN_UTILS_H__
