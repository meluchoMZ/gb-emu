/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#ifndef __ARITH_UTILS_H__
#define __ARITH_UTILS_H__

#include <stdbool.h>
#include <stdint.h>

/* Carry utils */

/**
 * Computes carry of a sum of two 8 bit numbers
 */
bool compute8BitCarry(uint8_t a, uint8_t b);

/**
 * Computes carry of a sum of two 8 bit numbers and an accumulated carry
 */
bool compute8BitCarryWithCarry(uint8_t a, uint8_t b, uint8_t carry);

/**
 * Computes half carry of a sum of two 8 bit numbers
 */
bool compute8BitHalfCarry(uint8_t a, uint8_t b);

/**
 * Computes half carry of a sum of two 8 bit numbers and an accumulated carry
 */
bool compute8BitHalfCarryWithCarry(uint8_t a, uint8_t b, uint8_t carry);

/**
 * Computes borrowing of a sum of two 8 bit numbers
 */
bool compute8BitBorrowing(uint8_t a, uint8_t b);

/**
 * Computes borrowing of a sum of two 8 bit numbers and an accumulated carry
 */
bool compute8BitBorrowingWithCarry(uint8_t a, uint8_t b, uint8_t carry);

/**
 * Computes half bit borrowing of a sum of two 8 bit numbers
 */
bool compute8BitHalfBitBorrowing(uint8_t a, uint8_t b);

/**
 * Computes half bit borrowing of a sum of two 8 bit numbers and an accumulated carry
 */
bool compute8BitHalfBitBorrowingWithCarry(uint8_t a, uint8_t b, uint8_t carry);

/**
 * Computes carry of a sum of two 16 bit numbers
 */
bool compute16BitCarry(uint16_t a, uint16_t b);

/**
 * Computes half carry of a sum of two 16 bit numbers
 */
bool compute16BitHalfCarry(uint16_t a, uint16_t b);

/**
 * Computes borrowing of a sum of two 16 bit numbers
 */
bool compute16BitBorrowing(uint16_t a, uint16_t b);

/**
 * Computes half bit borrowing of a sum of two 16 bit numbers
 */
bool compute16BitHalfBitBorrowing(uint16_t a, uint16_t b);

#endif //__ARITH_UTILS_H__
