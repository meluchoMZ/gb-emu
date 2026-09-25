/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "../../src/soc/bin_utils.h"

#include <ctest_api.h>
#include <stdint.h>

TEST(Binutils, extractMsb8Bit, "Tests the msb function works properly")
{
	uint16_t data = 0xDA4E;
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xDA, msb(data));
}

TEST(Binutils, extractLsb8Bit, "Tests the lsb function works properly")
{
	uint16_t data = 0xDA4E;
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x4E, lsb(data));
}

TEST(Binutils, toUint16, "Tests the toUint16 function works properly")
{
	uint16_t data = 0xDA4E;
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(data, toUint16(0xDA, 0x4E));
}
