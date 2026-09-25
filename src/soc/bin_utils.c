/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "bin_utils.h"

#include <stdint.h>

uint8_t msb(uint16_t value)
{
	return (uint8_t) ((value & 0xFF00) >> 8);
}

uint8_t lsb(uint16_t value)
{
	return (uint8_t) ((value & 0xFF));
}

uint16_t toUint16(uint8_t msb, uint8_t lsb)
{
	return (uint16_t) ((((msb << 8)) & 0xFF00) | lsb);
}
