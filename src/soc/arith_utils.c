/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include <stdbool.h>
#include <stdint.h>

/* Carry utils */

bool compute8BitCarry(uint8_t a, uint8_t b)
{
	return ((uint16_t) a + (uint16_t) b) > 0xFF;
}

bool compute8BitHalfCarry(uint8_t a, uint8_t b)
{
	return ((a & 0x0F) + (b & 0x0F)) > 0x0F;
}

bool compute8BitBorrowing(uint8_t a, uint8_t b)
{
	return a < b;
}

bool compute8BitHalfBitBorrowing(uint8_t a, uint8_t b)
{
	return (a & 0x0F) < (b & 0x0F);
}

bool compute16BitCarry(uint16_t a, uint16_t b)
{
	return ((uint32_t) a + (uint32_t) b) > 0xFFFF;
}

bool compute16BitHalfCarry(uint16_t a, uint16_t b)
{
	return ((a & 0x00FF) + (b & 0x00FF)) > 0x00FF;
}

bool compute16BitBorrowing(uint16_t a, uint16_t b)
{
	return a < b;
}

bool compute16BitHalfBitBorrowing(uint16_t a, uint16_t b)
{
	return (a & 0x00FF) < (b & 0x00FF);
}

