/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#ifndef __ARITH_UTILS_H__
#define __ARITH_UTILS_H__

#include <stdbool.h>
#include <stdint.h>

/* Carry utils */

bool compute8BitCarry(uint8_t a, uint8_t b);

bool compute8BitHalfCarry(uint8_t a, uint8_t b);

bool compute8BitBorrowing(uint8_t a, uint8_t b);

bool compute8BitHalfBitBorrowing(uint8_t a, uint8_t b);

bool compute16BitCarry(uint16_t a, uint16_t b);

bool compute16BitHalfCarry(uint16_t a, uint16_t b);

bool compute16BitBorrowing(uint16_t a, uint16_t b);

bool compute16BitHalfBitBorrowing(uint16_t a, uint16_t b);

#endif //__ARITH_UTILS_H__
