/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "../../../src/soc/cpu.h"
#include "../../../src/soc/cpu_instructions.h"
#include "../../../src/soc/memory.h"

#include "utils.h"

#include <ctest_api.h>

TEST(OtherOperations, nop_0x00, "Tests the NOP operation")
{
	INIT_ENV;
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.PC);
	EXEC(0x0, 0x0)
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x01, cpu.PC);
}

TEST(OtherOperations, halt_0xDD, "Tests the HALT operation")
{
	INIT_ENV;
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.PC);
	EXEC(0xD, 0xD);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.PC);
}
